#include "os.h"
#include "thread.h"

typedef struct
{
	thread_worker fnc;
	void * user;
} thread_info;

static DWORD WINAPI ThreadWrapper(
	void *pv)
{
	thread_info * pi = (thread_info *)pv;

	DWORD res = (DWORD)pi->fnc(pi->user);

	free(pi);

	return res;
}

gd_thread_t threadNew(
	thread_worker fnc, 
	void * user)
{
	thread_info * pi = (thread_info *)malloc(sizeof(thread_info));
	pi->fnc = fnc;
	pi->user = user;

	HANDLE h = CreateThread(NULL, 0, ThreadWrapper, pi, 0, NULL);

	return (gd_thread_t)h;
}

void threadJoin(gd_thread_t t)
{
	WaitForSingleObject((HANDLE)t, INFINITE);
}
