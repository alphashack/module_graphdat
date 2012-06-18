#include "os.h"
#include "socket.h"
#include <fcntl.h>

int socketGetLastError()
{
	return errno;
}

char * socketGetStringError(int error)
{
	return strerror(error);
}

char * socketGetLastStringError()
{
	return strerror(errno);
}

void socketDelStringError(void * dummy)
{
}

int socketNew()
{
	return socket(AF_UNIX, SOCK_STREAM, 0);
}

int socketClose(int sock)
{
	return close(sock);
}

int socketCleanup(char* config)
{
	return remove(config);
}

int socketSetNonBlock(int sock)
{
	return fcntl(sock, F_SETFL, O_NONBLOCK);
}

int socketSetPermission(char * config)
{
    char mode[] = "0777";
    int i = strtol(mode, 0, 8);
    return chmod(config, i);
}

int socketConnect(int sock, char* config)
{
	struct sockaddr_un serv_addr;
	int servlen;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, config);
	servlen = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path) + 1;
	return connect(sock, (struct sockaddr *)&serv_addr, servlen);
}

int socketBind(int sock, char* config)
{
	struct sockaddr_un serv_addr;
    int servlen;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, config);
    servlen = sizeof(serv_addr.sun_family) + strlen(serv_addr.sun_path) + 1;
    return bind(sock, (struct sockaddr *)&serv_addr, servlen);
}

int socketWrite(int sock, void* data, int len)
{
	return write(sock, data, len);
}
