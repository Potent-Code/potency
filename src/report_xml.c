#include "potency.h"
#include "report.h"
#include "report_xml.h"

#include <time.h>
#include <stdbool.h>
#include <string.h>

static potency_test_case* currentTestCase = NULL;
static bool caseTagOpen = false;

char* potency_escape_xml(const char* unescaped, char* escaped, const size_t escapedLength)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; (i < strlen(unescaped)) && (j < (escapedLength - 7)); i++)
	{
		// escape codes at http://www.w3.org/TR/xml/#dt-escape
		switch(unescaped[i])
		{
			case '<':
				escaped[j++] = '&';
				escaped[j++] = 'l';
				escaped[j++] = 't';
				escaped[j++] = ';';
				break;
			case '>':
				escaped[j++] = '&';
				escaped[j++] = 'g';
				escaped[j++] = 't';
				escaped[j++] = ';';
				break;
			case '&':
				escaped[j++] = '&';
				escaped[j++] = 'a';
				escaped[j++] = 'm';
				escaped[j++] = 'p';
				escaped[j++] = ';';
				break;
			case '"':
				escaped[j++] = '&';
				escaped[j++] = 'q';
				escaped[j++] = 'u';
				escaped[j++] = 'o';
				escaped[j++] = 't';
				escaped[j++] = ';';
				break;
			case '\'':
				escaped[j++] = '&';
				escaped[j++] = 'a';
				escaped[j++] = 'p';
				escaped[j++] = 'o';
				escaped[j++] = 's';
				escaped[j++] = ';';
				break;
			default:
				escaped[j++] = unescaped[i];
		}
	}

	// null terminate
	escaped[j] = 0;

	return escaped;
}

void potency_print_report_header_xml(const char* testSuite)
{
	const size_t escapedXMLLength = 4096;
	char escapedXML[escapedXMLLength];
	fprintf(reportFileHandle, "<potency>\n");
	fprintf(reportFileHandle, "\t<version>%s</version>\n", potency_escape_xml(PACKAGE_VERSION, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t<test_suite>%s</test_suite>\n", potency_escape_xml(testSuite, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t<time>%llu</time>\n", (unsigned long long)time(NULL));
}

void potency_print_benchmarks_xml()
{
	potency_test_case_list* currentTestCase = potency_get_test_case_list();
	const size_t escapedXMLLength = 4096;
	char escapedXML[escapedXMLLength];

	fprintf(reportFileHandle, "\t<benchmarks>\n");
	while (currentTestCase != NULL)
	{
		if (!potencySettings.filteringOn || currentTestCase->testCase->runTestCase)
		{
			fprintf(reportFileHandle, "\t\t<test_case>\n");
			fprintf(reportFileHandle, "\t\t\t<test_case_name>%s</test_case_name>\n", potency_escape_xml(currentTestCase->testCase->name, escapedXML, escapedXMLLength));
			fprintf(reportFileHandle, "\t\t\t<run_time>%g</run_time>\n", currentTestCase->testCase->runTime);
			fprintf(reportFileHandle, "\t\t</test_case>\n");
		}
		currentTestCase = currentTestCase->next;
	}
	fprintf(reportFileHandle, "\t</benchmarks>\n");
}

void potency_print_report_xml()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	if (caseTagOpen)
	{
		fprintf(reportFileHandle, "\t</test_case>\n");
	}
	
	potency_print_benchmarks_xml();

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

		const size_t escapedXMLLength = 4096;
		char escapedXML[escapedXMLLength];
		fprintf(reportFileHandle, "\t<test_case>\n");
		fprintf(reportFileHandle, "\t\t<test_case_name>%s</test_case_name>\n", potency_escape_xml(testCase->name, escapedXML, escapedXMLLength));
		caseTagOpen = true;
	}
}


void potency_print_assertion_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);

	const size_t escapedXMLLength = 4096;
	char escapedXML[escapedXMLLength];

	fprintf(reportFileHandle, "\t\t<assertion>\n");
	fprintf(reportFileHandle, "\t\t\t<file>%s</file>\n", potency_escape_xml(file, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t\t\t<line>%u</line>\n", line);
	fprintf(reportFileHandle, "\t\t\t<expression>CHECK(%s)</expression>\n", potency_escape_xml(expression, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t\t</assertion>\n");
}

void potency_print_requirement_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_xml(testCase);

	const size_t escapedXMLLength = 4096;
	char escapedXML[escapedXMLLength];

	fprintf(reportFileHandle, "\t\t<assertion>\n");
	fprintf(reportFileHandle, "\t\t\t<file>%s</file>\n", potency_escape_xml(file, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t\t\t<line>%u</line>\n", line);
	fprintf(reportFileHandle, "\t\t\t<expression>REQUIRE(%s)</expression>\n", potency_escape_xml(expression, escapedXML, escapedXMLLength));
	fprintf(reportFileHandle, "\t\t</assertion>\n");
}
