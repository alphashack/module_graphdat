#include "os.h"
#include "socket.h"

int socketGetLastError()
{
	return WSAGetLastError();
}

char * socketGetStringError(int error)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf,
		0,
		NULL);
	return (char *)lpMsgBuf;
}

char * socketGetLastStringError()
{
	return socketGetStringError(WSAGetLastError());
}

void socketDelStringError(void * str)
{
	LocalFree(str);
}

int socketNew()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

int socketCleanup(char *)
{
	return 0;
}

int socketClose(int sock)
{
	return closesocket(sock);
}

int socketSetNonBlock(int sock)
{
	unsigned long opt = 1;
	return ioctlsocket(sock, FIONBIO, &opt);
}

int socketSetPermission(char *)
{
	return 0;
}

int socketConnect(int sock, char * config)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(atoi(config));
	return connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

int socketBind(int sock, char * config)
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(atoi(config));
	return bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

int socketWrite(int sock, void* data, int len)
{
	return send(sock, (const char *)data, len, 0);
}
