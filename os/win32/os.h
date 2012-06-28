#ifndef GD_OS_H
#define GD_OS_H

#include <Windows.h>
#include <WinSock.h>
#include <malloc.h>
#include <string.h>

#define bool int
#define true 1
#define false 0

#define strerror_r strerror_s

void usleep(int usec);

#endif // GD_OS_H
