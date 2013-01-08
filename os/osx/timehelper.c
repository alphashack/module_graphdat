#include "timehelper.h"
#include <mach/clock.h>
#include <mach/mach.h>

int64_t get_ms()
{
    clock_serv_t cclock;
    mach_timespec_t ts;

    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &ts);
    mach_port_deallocate(mach_task_self(), cclock);

    int64_t    s = ts.tv_sec;
    int64_t nsec = ts.tv_nsec;
    return (s * 1000LL) + (nsec / 1000000LL);
}

