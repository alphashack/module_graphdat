#ifndef GD_MUTEX_H
#define GD_MUTEX_H

typedef void * gd_mutex_t;

gd_mutex_t mutexNew();
void mutexDel(gd_mutex_t m);
void mutexAcquire(gd_mutex_t m);
void mutexRelease(gd_mutex_t m);

#endif // GD_MUTEX_H
