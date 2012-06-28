#ifndef GD_SOCKET_H
#define GD_SOCKET_H

int socketGetLastError();
char * socketGetStringError(int);
char * socketGetLastStringError();
void socketDelStringError(void *);
int socketNew();
int socketClose(int);
int socketCleanup(char*);
int socketSetNonBlock(int);
int socketBind(int, char*);
int socketSetPermission(char*);
int socketConnect(int, char*);
int socketListen(int);
int socketSelect(int, int*, int, long, long);
int socketAccept(int);
int socketRead(int, char *, int);
int socketWrite(int, void*, int);

#endif // GD_SOCKET_H
