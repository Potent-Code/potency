#include "potency.h"
#include "report.h"
#include "report_xml.h"

#include <stdbool.h>

static potency_test_case* currentTestCase = NULL;
static bool caseTagOpen = false;

void potency_print_report_header_xml(const char* testSuite)
{
	fprintf(reportFileHandle, "<potency>\n");
	fprintf(reportFileHandle, "\t<test_suite>%s</test_suite>\n", testSuite);
}

void potency_print_report_xml()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (caseTagOpen)
	{
		fprintf(reportFileHandle, "\t</test_case>\n");
	}

	fprintf(reportFileHandle, "\t<statistics>\n");
	fprintf(reportFileHandle, "\t\t<assertions>%u</assertions>\n", stats.assertions);
	fprintf(reportFileHandle, "\t\t<passed_assertions>%u</passed_assertions>\n", stats.passedAssertions);
	fprintf(reportFileHandle, "\t\t<failed_assertions>%u</failed_assertions>\n", stats.failedAssertions);
	fprintf(reportFileHandle, "\t\t<success_percentage>%.2f</success_percentage>\n", stats.successPercentage);
	fprintf(reportFileHandle, "\t\t<test_cases>%u/%u</test_cases>\n", stats.testCasesRan, stats.testCases);
	fprintf(reportFileHandle, "\t</statistics>\n");
}

void potency_print_report_footer_xml()
{
	fprintf(reportFileHandle, "</potency>\n");
}

void potency_print_test_case_xml(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		if (currentTestCase != NULL)
		{
			fprintf(reportFileHandle, "\t</test_case>\n");
			caseTagOpen = false;
		}
		currentTestCase = testCase;
		fprintf(reportFileHandle, "\t<test_case>\n");
		fprintf(reportFileHandle, "\t\t<test_case_name>%s</test_case_name>\n", testCase->name);
		caseTagOpen = true;
	}
}


void potency_print_assertion_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);
	fprintf(reportFileHandle, "\t\t<assertion>\n");
	fprintf(reportFileHandle, "\t\t\t<file>%s</file>\n", file);
	fprintf(reportFileHandle, "\t\t\t<line>%u</line>\n", line);
	fprintf(reportFileHandle, "\t\t\t<expression>CHECK(%s)</expression>\n", expression);
	fprintf(reportFileHandle, "\t\t</assertion>\n");
}

void potency_print_requirement_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);
	fprintf(reportFileHandle, "\t\t<assertion>\n");
	fprintf(reportFileHandle, "\t\t\t<file>%s</file>\n", file);
	fprintf(reportFileHandle, "\t\t\t<line>%u</line>\n", line);
	fprintf(reportFileHandle, "\t\t\t<expression>REQUIRE(%s)</expression>\n", expression);
	fprintf(reportFileHandle, "\t\t</assertion>\n");
}
