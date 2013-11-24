#include "report.h"
#include "report_json.h"

potency_test_case* currentTestCase;

void potency_print_report_header_json(const char* testSuite)
{
	printf("{\n");
	(void)testSuite;
}

void potency_print_report_json()
{
	test_statistics stats;
	collect_statistics(&stats);

	printf("\t\"statistics\": {\n");
	printf("\t\t\"testCases\":%u,\n", stats.testCases);
	printf("\t\t\"assertions\":%u,\n", stats.assertions);
	printf("\t\t\"passedAssertions\":%u,\n", stats.passedAssertions);
	printf("\t\t\"failedAssertions\":%u,\n", stats.failedAssertions);
	printf("\t\t\"successPercentage\":%.2f\n", stats.successPercentage);
	printf("\t}\n");
}

void potency_print_report_footer_json()
{
	printf("{\n");
}

void potency_print_test_case_json(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		currentTestCase = testCase;
	}
}

void potency_print_assertion_json(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_json(testCase);
	(void)file;
	(void)line;
	(void)expression;
}

void potency_print_requirement_json(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_json(testCase);
	(void)file;
	(void)line;
	(void)expression;
}
