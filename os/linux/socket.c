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
	return strerror(errorno);
}

int socketNew()
{
	return socket(AF_UNIX, SOCK_STREAM, 0);
}

void socketClose(int sock)
{
	close(sock);
}

void socketCleanup(char* config)
{
	remove(config);
}

void socketSetNonBlock(int sock)
{
	return fcntl(sock, F_SETFL, O_NONBLOCK);
}

int socketSetPermission(char * config)
{
    char mode[] = "0777";
    int i = strtol(mode, 0, 8);
    return chmod(file, i);
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
}s

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
