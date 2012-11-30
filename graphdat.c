#include "graphdat.h"
#include "list.h"

#include "os.h"
#include "mutex.h"
#include "thread.h"
#include "socket.h"

#include "./msgpack/src/msgpack.h"

#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#ifndef GRAPHDAT_WORKER_LOOP_SLEEP
// GRAPHDAT_WORKER_LOOP_SLEEP usec
#define GRAPHDAT_WORKER_LOOP_SLEEP 100000
#endif

#define VERBOSE_LOGGING false

static bool s_init = false;
static bool s_running = true;
static char * s_sockconfig = NULL;
static char * s_source = NULL;
static size_t s_sourcelen;
static bool s_lastwaserror = false;
static bool s_lastwritesuccess = false;
static list_t s_requests;

static gd_mutex_t s_mux = NULL;
static gd_thread_t s_thread = NULL;

static int s_sock = -1;

void graphdat_send(char* method, size_t methodlen, char* uri, size_t urilen, char* host, size_t hostlen, double msec, logger_delegate_t logger, void * log_context);

void socket_close() {
	socketClose(s_sock);
	s_sock = -1;
}

void del_request(request_t * req) {
	free(req->log_context);
	free(req->host);
	free(req->uri);
	free(req->method);
	free(req);
}

void dlg_del_request(void * item) {
	request_t * req = (request_t *)item;
	del_request(req);
}

void socket_term(logger_delegate_t logger, void * log_context) {
        if(s_init) {
            s_running = false;
            threadJoin(s_thread);
			socket_close();
			free(s_sockconfig);
			free(s_source);
            listDel(s_requests, dlg_del_request);
        }
}

bool socket_connect(logger_delegate_t logger, void * log_context) {
	if(s_sock < 0)
	{
		s_sock = socketNew();
		if (s_sock < 0)
		{
			if(!s_lastwaserror || VERBOSE_LOGGING) {
				char * msg = socketGetLastStringError();
				logger(ERROR_MESSAGE, log_context, "graphdat error: could not create socket '%s' - [%d] %s", s_sockconfig, s_sock, msg);
				socketDelStringError(msg);
				s_lastwaserror = true;
			}
			return false;
		}

		int result = socketConnect(s_sock, s_sockconfig);
		if(result < 0)
		{
			if(!s_lastwaserror || VERBOSE_LOGGING) {
				int err = socketGetLastError();
				char * msg = socketGetStringError(err);
				logger(ERROR_MESSAGE, log_context, "graphdat error: could not connect socket '%s' (%d) - [%d] %s", s_sockconfig, s_sock, err, msg);
				socketDelStringError(msg);
				s_lastwaserror = true;
			}
			socket_close();
			return false;
		}

		result = socketSetNonBlock(s_sock);
		if(result < 0)
		{
			if(!s_lastwaserror || VERBOSE_LOGGING) {
				int err = socketGetLastError();
				char * msg = socketGetStringError(err);
				logger(ERROR_MESSAGE, log_context, "graphdat error: could set non blocking socket '%s' (%d) - [%d] %s", s_sockconfig, s_sock, err, msg);
				socketDelStringError(msg);
				s_lastwaserror = true;
			}
			socket_close();
			return false;
		}

		if(VERBOSE_LOGGING)
		{
			logger(INFORMATION_MESSAGE, log_context, "graphdat info: soket connected '%s' (%d)", s_sockconfig, s_sock);
		}
	}
	return true;
}

bool socket_check(logger_delegate_t logger, void * log_context) {
	if(!s_init) {
		if(!s_lastwaserror) {
			logger(ERROR_MESSAGE, log_context, "graphdat error: not initialised");
			s_lastwaserror = true;
		}
		return false;
	}
	return socket_connect(logger, log_context);
}

void* worker(void* arg)
{
	request_t *req;

	while(s_running) {
		mutexAcquire(s_mux);
		req = (request_t *)listRemoveFront(s_requests);
		mutexRelease(s_mux);

		if(req != NULL) {
			graphdat_send(req->method, req->methodlen, req->uri, req->urilen, req->host, req-> hostlen, req->msec, req->logger, req->log_context);
			del_request(req);
		}
		else
		{
			usleep(GRAPHDAT_WORKER_LOOP_SLEEP);
		}
	}
	return NULL;
}

void socket_init(char * config, size_t configlen, char* source, size_t sourcelen, logger_delegate_t logger, void * log_context) {
	s_sockconfig = (char *)malloc(configlen + 1);
	memcpy(s_sockconfig, config, configlen);
	s_sockconfig[configlen] = 0;
	s_sock = -1;
	s_source = (char *)malloc(sourcelen + 1);
	s_sourcelen = sourcelen;
	memcpy(s_source, source, sourcelen);
	s_source[sourcelen] = 0;
	s_requests = listNew();
	s_thread = threadNew(worker, NULL);
	s_init = true;
}

void socket_send(char * data, size_t len, logger_delegate_t logger, void * log_context) {
	if(!socket_check(logger, log_context)) return;

	int datalen = (int)len;
	if(datalen < 0)
	{
		logger(ERROR_MESSAGE, log_context, "graphdat error: could not write socket '%s' (%d) - data too long", s_sockconfig, s_sock);
		return;
	}

	int nlen = htonl(datalen);

	int wrote = socketWrite(s_sock, &nlen, sizeof(nlen));
	if(wrote < 0)
	{
		char * msg = socketGetLastStringError();
		logger(ERROR_MESSAGE, log_context, "graphdat error: could not write socket '%s' (%d) - [%d] %s", s_sockconfig, s_sock, wrote, msg);
		socketDelStringError(msg);
		socket_close();
		s_lastwritesuccess = false;
	}
    else
	{
		wrote = socketWrite(s_sock, data, datalen);
		if(wrote < 0)
		{
			char * msg = socketGetLastStringError();
			logger(ERROR_MESSAGE, log_context, "graphdat error: could not write socket '%s' (%d) - [%d] %s", s_sockconfig, s_sock, wrote, msg);
			socketDelStringError(msg);
			socket_close();
			s_lastwritesuccess = false;
		}
		else
		{
			if(!s_lastwritesuccess)
			{
				logger(SUCCESS_MESSAGE, log_context, "graphdat: sending data on socket '%s' (%d)", s_sockconfig, s_sock);
				s_lastwritesuccess = true;
			}
			s_lastwaserror = false;
		}

		if(VERBOSE_LOGGING)
		{
			logger(INFORMATION_MESSAGE, log_context, "graphdat info: socket sent %d bytes to '%s' (%d)", wrote, s_sockconfig, s_sock);
		}
	}
}

void graphdat_init(char * config, size_t configlen, char* source, size_t sourcelen, logger_delegate_t logger, void * log_context) {
	s_mux = mutexNew();
	socket_init(config, configlen, source, sourcelen, logger, log_context);
}

void graphdat_term(logger_delegate_t logger, void * log_context) {
	socket_term(logger, log_context);
	mutexDel(s_mux);
}

void graphdat_send(char* method, size_t methodlen, char* uri, size_t urilen, char* host, size_t hostlen, double msec, logger_delegate_t logger, void * log_context) {
	msgpack_sbuffer* buffer = msgpack_sbuffer_new();
	msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);

	// format route
	int routelen;
	char * route;
	if(methodlen > 0)
	{
		routelen = urilen + methodlen + 1; // len including space
		route = (char *)malloc(routelen);
		memcpy(route, method, methodlen);
		route[methodlen] = ' '; // append space
		memcpy(route + methodlen + 1, uri, urilen);
	}
	else
	{
		routelen = urilen;
		route = (char *)malloc(routelen);
		memcpy(route, uri, urilen);
	}

	msgpack_pack_map(pk, 4); // timestamp, type, host, route, responsetime, source
	// timestamp
	msgpack_pack_raw(pk, 9);
	msgpack_pack_raw_body(pk, "timestamp", 9);
	msgpack_pack_int(pk, 1);
	// type
	msgpack_pack_raw(pk, 4);
	msgpack_pack_raw_body(pk, "type", 4);
	msgpack_pack_raw(pk, 6);
	msgpack_pack_raw_body(pk, "Sample", 6);
	// host
	msgpack_pack_raw(pk, 4);
	msgpack_pack_raw_body(pk, "host", 4);
	msgpack_pack_raw(pk, hostlen);
	msgpack_pack_raw_body(pk, host, hostlen);
	// route
	msgpack_pack_raw(pk, 5);
	msgpack_pack_raw_body(pk, "route", 5);
	msgpack_pack_raw(pk, routelen);
	msgpack_pack_raw_body(pk, route, routelen);
	// responsetime
	msgpack_pack_raw(pk, 12);
	msgpack_pack_raw_body(pk, "responsetime", 12);
	msgpack_pack_double(pk, msec);
	// source
	msgpack_pack_raw(pk, 6);
	msgpack_pack_raw_body(pk, "source", 6);
	msgpack_pack_raw(pk, s_sourcelen);
	msgpack_pack_raw_body(pk, s_source, s_sourcelen);

	socket_send(buffer->data, buffer->size, logger, log_context);

	msgpack_sbuffer_free(buffer);
	msgpack_packer_free(pk);

	free(route);
}

void graphdat_store(char* method, size_t methodlen, char* uri, size_t urilen, char* host, size_t hostlen, double msec, logger_delegate_t logger, void * log_context, size_t log_context_len) {
	request_t *req = (request_t *)malloc(sizeof(request_t));
	// method
	req->method = (char *)malloc(methodlen);
	memcpy(req->method, method, methodlen);
	req->methodlen = methodlen;
	// uri
	req->uri = (char *)malloc(urilen);
	memcpy(req->uri, uri, urilen);
	req->urilen = urilen;
	// host
	req->host = (char *)malloc(hostlen);
	memcpy(req->host, host, hostlen);
	req->hostlen = hostlen;
	// msec
	req->msec = msec;
	// log
	req->logger = logger;
	if(log_context != NULL)
	{
		req->log_context = malloc(log_context_len);
		memcpy(req->log_context, log_context, log_context_len);
	}
	else
	{
		req->log_context = NULL;
	}

	mutexAcquire(s_mux);
	listAppendBack(s_requests, req);
	mutexRelease(s_mux);
}
