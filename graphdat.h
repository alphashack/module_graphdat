#ifndef GRAPHDAT_H
#define GRAPHDAT_H

#include "os.h"

typedef enum _graphdat_log_t
{
	SUCCESS_MESSAGE,
	ERROR_MESSAGE,
	WARNING_MESSAGE,
	INFORMATION_MESSAGE
} graphdat_log_t;

typedef void (*logger_delegate_t)(graphdat_log_t type, void * user, const char * fmt, ...);

typedef struct {
	char* method;
	size_t methodlen;
	char* uri;
	size_t urilen;
	double msec;
	logger_delegate_t logger;
	void * log_context;
} request_t;

void graphdat_init(char * config, size_t configlen, char* source, size_t sourcelen, logger_delegate_t logger, void * log_context);
void graphdat_term(logger_delegate_t logger, void * log_context);
void graphdat_store(char* method, size_t methodlen, char* uri, size_t urilen, double msec, logger_delegate_t logger, void * log_context, size_t log_context_len);

#endif /* GRAPHDAT_H */

