#include "potency.h"
#include "report.h"
#include "report_json.h"

#include <time.h>
#include <stdbool.h>

static potency_test_case* currentTestCase = NULL;
bool testCasesArray = false;
static bool caseTagOpen = false;

void potency_print_report_header_json(const char* testSuite)
{
	fprintf(reportFileHandle, "{\n");
	fprintf(reportFileHandle, "\t\"test_suite\":\"%s\",\n", testSuite);
	fprintf(reportFileHandle, "\t\"time\":%llu,\n", (unsigned long long)time(NULL));
}

void potency_print_report_json()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (testCasesArray)
	{
		fprintf(reportFileHandle, "\n\t],\n");
	}

	fprintf(reportFileHandle, "\t\"statistics\": {\n");
	fprintf(reportFileHandle, "\t\t\"test_cases\": \"%u/%u\",\n", stats.testCasesRan, stats.testCases);
	fprintf(reportFileHandle, "\t\t\"assertions\": %u,\n", stats.assertions);
	fprintf(reportFileHandle, "\t\t\"passed_assertions\": %u,\n", stats.passedAssertions);
	fprintf(reportFileHandle, "\t\t\"failed_assertions\": %u,\n", stats.failedAssertions);
	fprintf(reportFileHandle, "\t\t\"success_percentage\": %.2f\n", stats.successPercentage);
	fprintf(reportFileHandle, "\t}\n");
}

void potency_print_report_footer_json()
{
	fprintf(reportFileHandle, "}\n");
}

void potency_print_test_case_json(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		if (currentTestCase == NULL)
		{
			fprintf(reportFileHandle, "\t\"test_cases\": [\n");
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
		fprintf(reportFileHandle, ",\n");
	}
	fprintf(reportFileHandle, "\t\t{\n");
	fprintf(reportFileHandle, "\t\t\t\"assertion\": {\n");
	fprintf(reportFileHandle, "\t\t\t\t\"file\": \"%s\",\n", file);
	fprintf(reportFileHandle, "\t\t\t\t\"line\": %u,\n", line);
	fprintf(reportFileHandle, "\t\t\t\t\"expression\": \"CHECK(%s)\"\n", expression);
	fprintf(reportFileHandle, "\t\t\t}\n");
	fprintf(reportFileHandle, "\t\t}");
}

void potency_print_requirement_json(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_json(testCase);
	if (caseTagOpen)
	{
		fprintf(reportFileHandle, ",\n");
	}
	fprintf(reportFileHandle, "\t\t{\n");
	fprintf(reportFileHandle, "\t\t\t\"assertion\": {\n");
	fprintf(reportFileHandle, "\t\t\t\t\"file\": \"%s\",\n", file);
	fprintf(reportFileHandle, "\t\t\t\t\"line\": %u,\n", line);
	fprintf(reportFileHandle, "\t\t\t\t\"expression\": \"REQUIRE(%s)\"\n", expression);
	fprintf(reportFileHandle, "\t\t\t}\n");
	fprintf(reportFileHandle, "\t\t}");
}
