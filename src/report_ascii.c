#include "report.h"
#include "report_ascii.h"

potency_test_case* currentTestCase = NULL;

void potency_print_report_header_ascii()
{
	printf("\n");
}

void potency_print_report_ascii()
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

void potency_print_report_footer_ascii()
{
	printf("\n");
}

void potency_print_test_case_ascii(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		currentTestCase = testCase;
		printf("%s:\n", currentTestCase->name);
	}
}

void potency_print_assertion_ascii(potency_test_case* testCase, const char* file, uint32_t line, const char* expression)
{
	potency_print_test_case_ascii(testCase);
	printf("*** %s (%u): assertion failed - CHECK(%s)\n", file, line, expression);
}

void potency_print_requirement_ascii(potency_test_case* testCase, const char* file, uint32_t line, const char* expression)
{
	potency_print_test_case_ascii(testCase);
	printf("!!! %s (%u): requirement failed - REQUIRE(%s)\n", file, line, expression);
}
