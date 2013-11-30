#include "potency.h"
#include "report.h"
#include "report_markdown.h"

static potency_test_case* currentTestCase = NULL;

void potency_print_report_header_markdown(const char* testSuite)
{
	fprintf(reportFileHandle, "# %s #  \n  \n", testSuite);
}

void potency_print_report_markdown()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	fprintf(reportFileHandle, "\n## Unit Test Report ##\n");
	fprintf(reportFileHandle, "> *  Success Percentage: %.2f%%\n", stats.successPercentage);
	fprintf(reportFileHandle, "> *  Passed Assertions: %u\n", stats.passedAssertions);
	fprintf(reportFileHandle, "> *  Failed Assertions: %u\n", stats.failedAssertions);
	fprintf(reportFileHandle, "> *  Total Assertions: %u\n", stats.assertions);
	fprintf(reportFileHandle, "> *  Test Cases: %u/%u\n", stats.testCasesRan, stats.testCases);
}

void potency_print_report_footer_markdown()
{
	fprintf(reportFileHandle, "\n");
}

void potency_print_test_case_markdown(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		currentTestCase = testCase;
		fprintf(reportFileHandle, "\n### %s: ###\n", currentTestCase->name);
	}
}

void potency_print_assertion_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	fprintf(reportFileHandle, "> 1. **%s (%u)**: *assertion failed* - `CHECK(%s)`  \n", file, line, expression);
}

void potency_print_requirement_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	fprintf(reportFileHandle, "> 1. **%s (%u)**: *assertion failed* - `REQUIRE(%s)`  \n", file, line, expression);
}
