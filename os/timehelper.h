#ifndef GD_TIME_H
#define GD_TIME_H

#include <stdint.h>

#define SEC_PER_DAY 86400LL
#define MS_PER_DAY (SEC_PER_DAY * 1000LL)

int64_t get_ms(void);

#endif // GD_TIME_H

