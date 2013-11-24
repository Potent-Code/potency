#include "potency.h"
#include "report.h"
#include "report_markdown.h"

void potency_print_report_markdown()
{
	test_statistics stats;
	collect_statistics(&stats);

	printf("Unit Test Report\n");
	printf("----------------\n\n");
	printf("*  Success Percentage: %.2f%%\n", stats.successPercentage);
	printf("*  Passed Assertions: %u\n", stats.passedAssertions);
	printf("*  Failed Assertions: %u\n", stats.failedAssertions);
	printf("*  Total Assertions: %u\n", stats.assertions);
	printf("*  Test Cases: %u\n", stats.testCases);
}
