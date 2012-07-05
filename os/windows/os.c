#include "os.h"

void usleep(int usec)
{
	Sleep(usec/1000);
}
