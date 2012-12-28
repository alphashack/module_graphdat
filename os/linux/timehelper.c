#include "timehelper.h"
#include <time.h>

int64_t get_ms()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	int64_t	s = ts.tv_sec;
	int64_t nsec = ts.tv_nsec;
	return (s * 1000LL) + (nsec / 1000000LL);
}

