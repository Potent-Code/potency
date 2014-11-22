#include "potency.h"
#include "report.h"

void potency_collect_statistics(test_statistics* stats)
{
	// initialize statistics struct
	stats->testCases = 0;
	stats->testCasesRan = 0;
	stats->assertions = 0;
	stats->passedAssertions = 0;
	stats->failedAssertions = 0;
	stats->successPercentage = 0.0;

	potency_test_case_list* currentTestCase = potency_get_test_case_list();

	while (currentTestCase != NULL)
	{
		stats->testCases++;

		if (potencySettings.filteringOn)
		{
			if (currentTestCase->testCase->runTestCase)
			{
				stats->testCasesRan++;
			}
		}
		else
		{
			stats->testCasesRan++;
		}

		stats->assertions += currentTestCase->testCase->assertions;
		stats->passedAssertions += currentTestCase->testCase->passedAssertions;
		stats->failedAssertions += currentTestCase->testCase->assertions - currentTestCase->testCase->passedAssertions;
		currentTestCase = currentTestCase->next;
	}

	if (stats->assertions != 0)
	{
		stats->successPercentage = ((double)stats->passedAssertions / (double)stats->assertions) * 100.0;
	}

	// this is a visual enhancement. We only display two digits after the decimal place, so
	// it might be the case that we round up to 100.00%. For verification purposes, we do not
	// want to round up past 99.99%, but only display 100.00% if passedAssertions == assertions
	if ((stats->successPercentage > 99.99) && (stats->passedAssertions != stats->assertions))
	{
		stats->successPercentage = 99.99;
	}
}

