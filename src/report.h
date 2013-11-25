#ifndef potency_report_h
#define potency_report_h

#include <stdint.h>

typedef struct _test_statistics
{
	uint32_t testCases;
	uint32_t assertions;
	uint32_t passedAssertions;
	uint32_t failedAssertions;
	double successPercentage;
} test_statistics;

extern void potency_collect_statistics(test_statistics* stats);

#endif // !potency_report_h
