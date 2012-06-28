#include "os.h"

int socketAccept(int sock)
{
	return accept(sock, NULL, NULL);
}

int socketSelect(int maxnumsocks, int* readsocks, int numreadsocks, long timeoutsec, long timeoutusec)
{
		fd_set fds;
		struct timeval timeout;
		timeout.tv_sec = timeoutsec;
		timeout.tv_usec = timeoutusec;
		FD_ZERO(&fds);
		for(int i = 0; i < numreadsocks; i++)
		{
			FD_SET(readsocks[i], &fds);
		}
		return select(maxnumsocks, &fds, NULL, NULL, &timeout);
}

int socketListen(int sock)
{
	return listen(sock, 1);
}

int socketRead(int sock, char * buf, int buflen)
{
	return recv(sock, buf, buflen, 0);
}
