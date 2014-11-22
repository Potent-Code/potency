#include "benchmark.h"

#include <stdlib.h>
#include <sys/time.h>

double potency_get_time()
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL))
	{
		return 0;
	}

	return (double)tv.tv_sec + ((double)tv.tv_usec * 1.0e-6);
}

