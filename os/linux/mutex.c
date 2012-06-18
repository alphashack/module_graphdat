#include "mutex.h"

#include <pthread.h>
#include <malloc.h>

gd_mutex_t mutexNew()
{
	// Note that mutex is used internally by mem_alloc so we will roll our own alloc here
	pthread_mutex_t * pm = malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(pm, NULL);

	return (gd_mutex_t)pm;
}

void mutexDel(gd_mutex_t m)
{
	free((pthread_mutex_t *)m);
}

void mutexAcquire(gd_mutex_t m)
{
	pthread_mutex_t * pm = (pthread_mutex_t *)m;

	pthread_mutex_lock(pm);
}

void mutexRelease(gd_mutex_t m)
{
	pthread_mutex_t * pm = (pthread_mutex_t *)m;

	pthread_mutex_unlock(pm);
}
