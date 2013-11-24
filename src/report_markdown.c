#include "potency.h"
#include "report.h"
#include "report_markdown.h"

#include <string.h>

void potency_print_report_header_markdown(const char* testSuite)
{
	printf("%s", testSuite);
	size_t i;
	for (i = 0; i < strlen(testSuite); i++)
	{
		printf("=");
	}
	printf("\n");
}

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

void potency_print_report_footer_markdown()
{

}

void potency_print_assertion_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	(void)testCase;
	(void)file;
	(void)line;
	(void)expression;
}

void potency_print_requirement_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	(void)testCase;
	(void)file;
	(void)line;
	(void)expression;
}
