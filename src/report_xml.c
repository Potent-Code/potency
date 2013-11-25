#include "potency.h"
#include "report.h"
#include "report_xml.h"

#include <stdbool.h>

static potency_test_case* currentTestCase = NULL;
static bool caseTagOpen = false;

void potency_print_report_header_xml(const char* testSuite)
{
	printf("<potency>\n");
	printf("\t<test_suite>%s</test_suite>\n", testSuite);
}

void potency_print_report_xml()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (caseTagOpen)
	{
		printf("\t</test_case>\n");
	}

	printf("\t<statistics>\n");
	printf("\t\t<assertions>%u</assertions>\n", stats.assertions);
	printf("\t\t<passed_assertions>%u</passed_assertions>\n", stats.passedAssertions);
	printf("\t\t<failed_assertions>%u</failed_assertions>\n", stats.failedAssertions);
	printf("\t\t<success_percentage>%.2f</success_percentage>\n", stats.successPercentage);
	printf("\t\t<test_cases>%u</test_cases>\n", stats.testCases);
	printf("\t</statistics>\n");
}

void potency_print_report_footer_xml()
{
	printf("</potency>\n");
}

void potency_print_test_case_xml(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		if (currentTestCase != NULL)
		{
			printf("\t</test_case>\n");
			caseTagOpen = false;
		}
		currentTestCase = testCase;
		printf("\t<test_case>\n");
		printf("\t\t<test_case_name>%s</test_case_name>\n", testCase->name);
		caseTagOpen = true;
	}
}


void potency_print_assertion_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);
	printf("\t\t<assertion>\n");
	printf("\t\t\t<file>%s</file>\n", file);
	printf("\t\t\t<line>%u</line>\n", line);
	printf("\t\t\t<expression>CHECK(%s)</expression>\n", expression);
	printf("\t\t</assertion>\n");
}

void potency_print_requirement_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);
	printf("\t\t<assertion>\n");
	printf("\t\t\t<file>%s</file>\n", file);
	printf("\t\t\t<line>%u</line>\n", line);
	printf("\t\t\t<expression>REQUIRE(%s)</expression>\n", expression);
	printf("\t\t</assertion>\n");
}
