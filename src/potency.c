#include "potency.h"
#include "report_markdown.h"
#include "report_json.h"
#include "report_xml.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

static potency_test_case_list* testCaseHead = NULL;

potency_output_function potency_print_assertion = NULL;		// output function for CHECK
potency_output_function potency_print_requirement = NULL;	// output function for REQUIRE
potency_setup_function potency_setup = NULL;				// user supplied set up function
potency_teardown_function potency_teardown = NULL;			// user supplied tear down function
FILE* reportFileHandle = NULL;								// file handle to output report to

potency_settings potencySettings;

void potency_add_setup_function(potency_setup_function _potency_setup)
{
	potency_setup = _potency_setup;
}

void potency_add_teardown_function(potency_teardown_function _potency_teardown)
{
	potency_teardown = _potency_teardown;
}

void potency_add_test_case( const char* name, const char* description, potency_test_case_function _run )
{
	potency_test_case* newTestCase = calloc(1, sizeof(*newTestCase));
	newTestCase->name = name;
	newTestCase->description = description;
	newTestCase->run = _run;
	newTestCase->runTestCase = false;

	if (testCaseHead == NULL)
	{
		testCaseHead = calloc(1, sizeof(*testCaseHead));
		testCaseHead->testCase = newTestCase;
		return;
	}

	potency_test_case_list* testCaseEnd = testCaseHead;
	while (testCaseEnd->next != NULL)
	{
		testCaseEnd = testCaseEnd->next;
	}

	potency_test_case_list* newTestCaseList = calloc(1, sizeof(*newTestCaseList));
	newTestCaseList->testCase = newTestCase;
	newTestCaseList->prev = testCaseEnd;
	testCaseEnd->next = newTestCaseList;

	return;
}

potency_test_case_list* potency_get_test_case_list()
{
	return testCaseHead;
}

static void potency_cleanup_test_cases()
{
	potency_test_case_list* currentTestCaseList = potency_get_test_case_list();

	while (currentTestCaseList != NULL)
	{
		free(currentTestCaseList->testCase);

		potency_test_case_list* toBeFreed = currentTestCaseList;
		currentTestCaseList = toBeFreed->next;
		free(toBeFreed);
	}
}

int main(int argc, char** argv)
{
	int i;
	potency_test_case_list* currentCase = potency_get_test_case_list();
	void(*potency_print_report)() = NULL;
	void(*potency_print_report_header)(const char*) = NULL;
	void(*potency_print_report_footer)() = NULL;

	// set default potencySettings
	potencySettings.threads = 1;
	potencySettings.format = outputFormatMarkdown;
	potencySettings.outputFile = NULL;
	potencySettings.verboseMode = false;
	potencySettings.listMode = false;
	potencySettings.helpMode = false;
	potencySettings.filteringOn = false;

	// process command line arguments
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			// get format
			if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--format") == 0))
			{
				if (strcmp(argv[i + 1], "markdown") == 0)
				{
					potencySettings.format = outputFormatMarkdown;
					i++;
					continue;
				}
				else if (strcmp(argv[i+1], "json") == 0)
				{
					potencySettings.format = outputFormatJSON;
					i++;
					continue;
				}
				else if (strcmp(argv[i+1], "xml") == 0)
				{
					potencySettings.format = outputFormatXML;
					i++;
					continue;
				}
				else
				{
					potencySettings.helpMode = true;
					break;
				}
			}
			else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--list") == 0))
			{
				potencySettings.listMode = true;
				// we don't break here in case help mode is actually desired
			}
			else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0))
			{
				potencySettings.outputFile = argv[i + 1];
				i++;
				continue;
			}
			else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--threads") == 0))
			{
				potencySettings.threads = atoi(argv[i+1]);
				i++;
				continue;
			}
			else if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0))
			{
				potencySettings.verboseMode = true;
			}
			else
			{
				potencySettings.helpMode = true;
				break;
			}
		}
		else
		{
			potencySettings.filteringOn = true;

			potency_test_case_list* _currentCase = currentCase;
			while (currentCase != NULL)
			{
				if (strstr(currentCase->testCase->name, argv[i]) != NULL)
				{
					currentCase->testCase->runTestCase = true;
				}
				currentCase = currentCase->next;
			}
			currentCase = _currentCase;
		}
	}

	// check if we need to print a help screen and exit
	if (potencySettings.helpMode)
	{
		printf("Usage: %s [OPTIONS]... [FILTERS]...\n", argv[0]);
		printf("OPTIONS\n");
		printf("  -f, --format\t\t\tSpecify output format. Must be one of markdown, json, or xml. Defaults to markdown.\n");
		printf("  -l, --list\t\t\tList known test cases\n");
		printf("  -o, --output\t\t\tOutput report to a file instead of standard output\n");
		printf("  -t, --threads\t\t\tSpecify number of threads to run. Must be at least one, and at most the number of test cases linked\n");
		printf("  -v, --verbose\t\t\tIncrease the amount of output\n");
		printf("  -h, --help\t\t\tThis help screen\n");
		printf("\nFILTERS\n");
		printf("  If any arguments are passed that do not begin with -, they are considered filters.\n");
		printf("  If no filters are supplied, all test cases will be executed. Otherwise, only test cases\n");
		printf("  whose names match at least one filter will be executed.\n");
		potency_cleanup_test_cases();
		return 0;
	}

	if (potencySettings.listMode)
	{
		while (currentCase != NULL)
		{
			printf("%-30s* %s\n", currentCase->testCase->name, currentCase->testCase->description);
			currentCase = currentCase->next;
		}
		potency_cleanup_test_cases();
		return 0;
	}

	// setup output functions
	switch (potencySettings.format)
	{
		case outputFormatJSON:
			potency_print_report_header = &potency_print_report_header_json;
			potency_print_report = &potency_print_report_json;
			potency_print_report_footer = &potency_print_report_footer_json;
			potency_print_assertion = &potency_print_assertion_json;
			potency_print_requirement = &potency_print_requirement_json;
			break;
		case outputFormatXML:
			potency_print_report_header = &potency_print_report_header_xml;
			potency_print_report = &potency_print_report_xml;
			potency_print_report_footer = &potency_print_report_footer_xml;
			potency_print_assertion = &potency_print_assertion_xml;
			potency_print_requirement = &potency_print_requirement_xml;
			break;
		case outputFormatMarkdown:
		default:
			potency_print_report_header = &potency_print_report_header_markdown;
			potency_print_report = &potency_print_report_markdown;
			potency_print_report_footer = &potency_print_report_footer_markdown;
			potency_print_assertion = &potency_print_assertion_markdown;
			potency_print_requirement = &potency_print_requirement_markdown;
			break;
	}

	// reset the current test case
	currentCase = potency_get_test_case_list();

	if (potencySettings.outputFile != NULL)
	{
		reportFileHandle = fopen(potencySettings.outputFile, "w+");
		if (reportFileHandle == NULL)
		{
			perror("Error - failed to open output file");
			potency_cleanup_test_cases();
			return 1;
		}
	}
	else
	{
		// default to standard output
		reportFileHandle = stdout;
	}

	// run setup function
	void* setup_ptr = NULL;
	if (potency_setup != NULL)
	{
		setup_ptr = (*potency_setup)();
	}

	// run test cases
	(*potency_print_report_header)(argv[0]);
	while (currentCase != NULL)
	{
		if (!potencySettings.filteringOn || (potencySettings.filteringOn && currentCase->testCase->runTestCase))
		{
			currentCase->testCase->run(currentCase->testCase);
		}

		currentCase = currentCase->next;
	}

	// output statistics
	(*potency_print_report)();
	(*potency_print_report_footer)();

	// run tear down function
	if (potency_teardown != NULL)
	{
		(*potency_teardown)(setup_ptr);
	}

	// close output report file handle
	if (reportFileHandle != stdout)
	{
		fclose(reportFileHandle);
	}

	// clean up
	potency_cleanup_test_cases();
	return 0;
}
