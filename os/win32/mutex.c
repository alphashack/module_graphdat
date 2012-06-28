#include "os.h"
#include "mutex.h"

gd_mutex_t mutexNew(void)
{
	HANDLE h = CreateMutex(NULL, FALSE, NULL);

	return (gd_mutex_t)h;
}

void mutexDel(gd_mutex_t m)
{
	CloseHandle((HANDLE)m);
}

void mutexAcquire(gd_mutex_t m)
{
	WaitForSingleObject((HANDLE)m, INFINITE);
}

void mutexRelease(gd_mutex_t m)
{
	ReleaseMutex((HANDLE)m);
}
