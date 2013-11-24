#include "potency.h"
#include "report.h"
#include "report_xml.h"

void potency_print_report_header_xml(const char* testSuite)
{
	(void)testSuite;
}

void potency_print_report_xml()
{
	test_statistics stats;
	collect_statistics(&stats);

	printf("<statistics>\n");
	printf("\t<assertions>%u</assertions>\n", stats.assertions);
	printf("\t<passedassertions>%u</passedassertions>\n", stats.passedAssertions);
	printf("\t<failedassertions>%u</failedassertions>\n", stats.failedAssertions);
	printf("\t<successpercentage>%.2f</successpercentage>\n", stats.successPercentage);
	printf("\t<testcases>%u</testcases>\n", stats.testCases);
	printf("</statistics>\n");
}

void potency_print_report_footer_xml()
{

}

void potency_print_assertion_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	(void)testCase;
	(void)file;
	(void)line;
	(void)expression;
}

void potency_print_requirement_xml(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	(void)testCase;
	(void)file;
	(void)line;
	(void)expression;
}
