#ifndef GD_OS_H
#define GD_OS_H

#include <Windows.h>
#include <WinSock.h>
#include <malloc.h>
#include <string.h>

#define strerror_r strerror_s

void usleep(int usec);

#endif // GD_OS_H
