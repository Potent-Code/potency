#include "potency.h"
#include "report.h"
#include "report_markdown.h"

#include <string.h>

static potency_test_case* currentTestCase = NULL;

void potency_print_report_header_markdown(const char* testSuite)
{
	printf("# %s #  \n  \n", testSuite);
}

void potency_print_report_markdown()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	printf("\n## Unit Test Report ##\n");
	printf("> *  Success Percentage: %.2f%%\n", stats.successPercentage);
	printf("> *  Passed Assertions: %u\n", stats.passedAssertions);
	printf("> *  Failed Assertions: %u\n", stats.failedAssertions);
	printf("> *  Total Assertions: %u\n", stats.assertions);
	printf("> *  Test Cases: %u\n", stats.testCases);
}

void potency_print_report_footer_markdown()
{
	printf("\n");
}

void potency_print_test_case_markdown(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		currentTestCase = testCase;
		printf("\n### %s: ###\n", currentTestCase->name);
	}
}

void potency_print_assertion_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	printf("> 1. **%s (%u)**: *assertion failed* - `CHECK(%s)`  \n", file, line, expression);
}

void potency_print_requirement_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	printf("> 1. **%s (%u)**: *assertion failed* - `REQUIRE(%s)`  \n", file, line, expression);
}
