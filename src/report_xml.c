#include "potency.h"
#include "report.h"
#include "report_xml.h"

void prepare_report_xml()
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
