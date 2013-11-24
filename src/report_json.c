#include "potency.h"
#include "report.h"
#include "report_json.h"

void prepare_report_json()
{
	test_statistics stats;
	collect_statistics(&stats);

	printf("{\n");
	printf("\t\"statistics\": {\n");
	printf("\t\t\"testCases\":%u,\n", stats.testCases);
	printf("\t\t\"assertions\":%u,\n", stats.assertions);
	printf("\t\t\"passedAssertions\":%u,\n", stats.passedAssertions);
	printf("\t\t\"failedAssertions\":%u,\n", stats.failedAssertions);
	printf("\t\t\"successPercentage\":%.2f\n", stats.successPercentage);
	printf("\t}\n");
	printf("}\n");
}
