#include "potency.h"
#include "report.h"
#include "report_ascii.h"

void prepare_report_ascii()
{
	test_statistics stats;
	collect_statistics(&stats);

	printf("\n");
	printf("-----------------------------------------------------------------\n");
	printf("|                        Unit Test Report                       |\n");
	printf("-----------------------------------------------------------------\n");
	printf("|  Success Percentage:%39.2f%%  |\n", stats.successPercentage);
	printf("|  Passed Assertions:%41u  |\n", stats.passedAssertions);
	printf("|  Failed Assertions:%41u  |\n", stats.failedAssertions);
	printf("|  Total Assertions:%42u  |\n", stats.assertions);
	printf("|  Test Cases:%48u  |\n", stats.testCases);
	printf("-----------------------------------------------------------------\n");
}
