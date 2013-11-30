#include "report.h"
#include "report_json.h"

#include <stdbool.h>

static potency_test_case* currentTestCase = NULL;
bool testCasesArray = false;
static bool caseTagOpen = false;

void potency_print_report_header_json(const char* testSuite)
{
	printf("{\n");
	printf("\t\"test_suite\":\"%s\",\n", testSuite);
	(void)testSuite;
}

void potency_print_report_json()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (testCasesArray)
	{
		printf("\n\t],\n");
	}

	printf("\t\"statistics\": {\n");
	printf("\t\t\"test_cases\": \"%u/%u\",\n", stats.testCasesRan, stats.testCases);
	printf("\t\t\"assertions\": %u,\n", stats.assertions);
	printf("\t\t\"passed_assertions\": %u,\n", stats.passedAssertions);
	printf("\t\t\"failed_assertions\": %u,\n", stats.failedAssertions);
	printf("\t\t\"success_percentage\": %.2f\n", stats.successPercentage);
	printf("\t}\n");
}

void potency_print_report_footer_json()
{
	printf("}\n");
}

void potency_print_test_case_json(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		if (currentTestCase == NULL)
		{
			printf("\t\"test_cases\": [\n");
			testCasesArray = true;
			caseTagOpen = false;
		}
		else
		{
			caseTagOpen = true;
		}
		currentTestCase = testCase;
	}
}

void potency_print_assertion_json(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_json(testCase);
	if (caseTagOpen)
	{
		printf(",\n");
	}
	printf("\t\t{\n");
	printf("\t\t\t\"assertion\": {\n");
	printf("\t\t\t\t\"file\": \"%s\",\n", file);
	printf("\t\t\t\t\"line\": %u,\n", line);
	printf("\t\t\t\t\"expression\": \"CHECK(%s)\"\n", expression);
	printf("\t\t\t}\n");
	printf("\t\t}");
}

void potency_print_requirement_json(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_json(testCase);
	if (caseTagOpen)
	{
		printf(",\n");
	}
	printf("\t\t{\n");
	printf("\t\t\t\"assertion\": {\n");
	printf("\t\t\t\t\"file\": \"%s\",\n", file);
	printf("\t\t\t\t\"line\": %u,\n", line);
	printf("\t\t\t\t\"expression\": \"REQUIRE(%s)\"\n", expression);
	printf("\t\t\t}\n");
	printf("\t\t}");
}
