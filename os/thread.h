#ifndef GD_THREAD_H
#define GD_THREAD_H

typedef void * gd_thread_t;

typedef void *(* thread_worker)(void *);

gd_thread_t threadNew(thread_worker fnc, void * user);
void threadJoin(gd_thread_t t);

#endif // GD_THREAD_H
