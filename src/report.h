#ifndef potency_report_h
#define potency_report_h

#include <stdint.h>

typedef struct _test_statistics
{
	uint32_t testCases;				// total number of test cases
	uint32_t testCasesRan;			// how many test cases we ran
	uint32_t assertions;			// total number of assertions
	uint32_t passedAssertions;		// total number of true assertions
	uint32_t failedAssertions;		// total number of failed assertions
	double successPercentage;		// passedAssertions / assertions * 100
} test_statistics;

extern void potency_collect_statistics(test_statistics* stats);

#endif // !potency_report_h
