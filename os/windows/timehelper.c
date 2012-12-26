#include "timehelper.h"
#include "os.h"
#include <time.h>

int64_t get_ms()
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	int64_t ns = (int64_t)ft.dwLowDateTime + ((int64_t)ft.dwHighDateTime << 32LL);

	// Move to Jan1, 1970
	ns -= 116444736000000000LL;

	// Convert to ms
	ns /= 10000LL;

	return ns;
}

