#include "thread.h"

#include <pthread.h>

gd_thread_t threadNew(thread_worker fnc, void * user)
{
	pthread_t t = (pthread_t)NULL;
	pthread_create(&t, NULL, fnc, user);

	return (gd_thread_t)t;
}

void threadJoin(gd_thread_t t)
{
	pthread_join((pthread_t)t, NULL);
}

