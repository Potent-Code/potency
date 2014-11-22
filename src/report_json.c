#include "potency.h"
#include "report.h"
#include "report_json.h"

#include <time.h>
#include <stdbool.h>
#include <string.h>

static potency_test_case* currentTestCase = NULL;
bool testCasesArray = false;
static bool caseTagOpen = false;

char* potency_escape_json(const char* unescaped, char* escaped, const size_t escapedLength)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; (i < strlen(unescaped)) && (j < (escapedLength - 3)); i++)
	{
		// escape codes at http://www.json.org/
		switch(unescaped[i])
		{
			case '\\':
			case '"':
				escaped[j++] = '\\';
				escaped[j++] = unescaped[i];
				break;
			default:
				escaped[j++] = unescaped[i];
		}
	}

	// null terminate
	escaped[j] = 0;

	return escaped;
}

void potency_print_report_header_json(const char* testSuite)
{
	const size_t escapedJSONLength = 4096;
	char escapedJSON[escapedJSONLength];
	fprintf(reportFileHandle, "{\n");
	fprintf(reportFileHandle, "\t\"test_suite\":\"%s\",\n", potency_escape_json(testSuite, escapedJSON, escapedJSONLength));
	fprintf(reportFileHandle, "\t\"time\":%llu,\n", (unsigned long long)time(NULL));
	fprintf(reportFileHandle, "\t\"potency_version\": \"%s\",\n", potency_escape_json(PACKAGE_VERSION, escapedJSON, escapedJSONLength));
}

void potency_print_benchmarks_json()
{
	potency_test_case_list* currentTestCase = potency_get_test_case_list();
	const size_t escapedJSONLength = 4096;
	char escapedJSON[escapedJSONLength];

	fprintf(reportFileHandle, "\t\"benchmarks\": {\n");

	bool benchmarkTagOpen = false;
	while (currentTestCase != NULL)
	{
		if (!potencySettings.filteringOn || currentTestCase->testCase->runTestCase)
		{
			if (benchmarkTagOpen)
			{
				fprintf(reportFileHandle, ",\n");
			}

			fprintf(reportFileHandle, "\t\t\"%s\": %g", potency_escape_json(currentTestCase->testCase->name, escapedJSON, escapedJSONLength), currentTestCase->testCase->runTime);
			benchmarkTagOpen = true;
		}
		currentTestCase = currentTestCase->next;
	}
	fprintf(reportFileHandle, "\n\t},\n");
}

void potency_print_report_json()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (testCasesArray)
	{
		fprintf(reportFileHandle, "\n\t],\n");
	}

	potency_print_benchmarks_json();

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

	const size_t escapedJSONLength = 4096;
	char escapedJSON[escapedJSONLength];

	fprintf(reportFileHandle, "\t\t{\n");
	fprintf(reportFileHandle, "\t\t\t\"assertion\": {\n");
	fprintf(reportFileHandle, "\t\t\t\t\"file\": \"%s\",\n", potency_escape_json(file, escapedJSON, escapedJSONLength));
	fprintf(reportFileHandle, "\t\t\t\t\"line\": %u,\n", line);
	fprintf(reportFileHandle, "\t\t\t\t\"expression\": \"CHECK(%s)\"\n", potency_escape_json(expression, escapedJSON, escapedJSONLength));
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

	const size_t escapedJSONLength = 4096;
	char escapedJSON[escapedJSONLength];

	fprintf(reportFileHandle, "\t\t{\n");
	fprintf(reportFileHandle, "\t\t\t\"assertion\": {\n");
	fprintf(reportFileHandle, "\t\t\t\t\"file\": \"%s\",\n", potency_escape_json(file, escapedJSON, escapedJSONLength));
	fprintf(reportFileHandle, "\t\t\t\t\"line\": %u,\n", line);
	fprintf(reportFileHandle, "\t\t\t\t\"expression\": \"REQUIRE(%s)\"\n", potency_escape_json(expression, escapedJSON, escapedJSONLength));
	fprintf(reportFileHandle, "\t\t\t}\n");
	fprintf(reportFileHandle, "\t\t}");
}

