#include "potency.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct _potency_test_case_list
{
	potency_test_case* testCase;
	struct _potency_test_case_list* next;
	struct _potency_test_case_list* prev;
} potency_test_case_list;

static potency_test_case_list* testCaseHead = NULL;

void potency_add_test_case( const char* name, const char* description, potency_test_case_function _run )
{
	potency_test_case* newTestCase = calloc(1, sizeof(*newTestCase));
	newTestCase->name = name;
	newTestCase->description = description;
	newTestCase->run = _run;

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

static void potency_cleanup_test_cases()
{
	potency_test_case_list* currentTestCaseList = testCaseHead;

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
	potency_test_case_list* currentCase = testCaseHead;

	// output mode
	typedef enum
	{
		outputFormatASCII,
		outputFormatMarkdown,
		outputFormatJSON,
		outputFormatXML
	} output_format;

	// command line arguments that begin with - represent flags
	struct _flags
	{
		size_t threads;			// number of threads to run test cases
		output_format format;	// what format to generate report in
		bool verboseMode;		// print extra output
		bool listMode;			// should we just list test cases and exit?
		bool helpMode;			// print the help screen
	} flags;

	// set default flags
	flags.threads = 1;
	flags.format = outputFormatASCII;
	flags.verboseMode = false;
	flags.listMode = false;
	flags.helpMode = false;

	// process command line arguments
	for (i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			// get format
			if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--format") == 0))
			{
				if (strcmp(argv[i+1], "ascii") == 0)
				{
					flags.format = outputFormatASCII;
					i++;
					continue;
				}
				else if (strcmp(argv[i+1], "markdown") == 0)
				{
					flags.format = outputFormatMarkdown;
					i++;
					continue;
				}
				else if (strcmp(argv[i+1], "json") == 0)
				{
					flags.format = outputFormatJSON;
					i++;
					continue;
				}
				else if (strcmp(argv[i+1], "xml") == 0)
				{
					flags.format = outputFormatXML;
					i++;
					continue;
				}
				else
				{
					flags.helpMode = true;
					break;
				}
			}
			else if ((strcmp(argv[i], "-l") == 0) || (strcmp(argv[i], "--list") == 0))
			{
				flags.listMode = true;
				// we don't break here in case help mode is actually desired
			}
			else if ((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--threads") == 0))
			{
				flags.threads = atoi(argv[i+1]);
				i++;
				continue;
			}
			else if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--verbose") == 0))
			{
				flags.verboseMode = true;
			}
			else
			{
				flags.helpMode = true;
				break;
			}
		}
		else
		{
			// process filters
		}
	}

	// check if we need to print a help screen and exit
	if (flags.helpMode)
	{
		printf("Usage: %s [OPTIONS]... [FILTERS]...\n", argv[0]);
		printf("OPTIONS\n");
		printf("  -f, --format\t\t\tSpecify output format. Must be one of ascii, markdown, json, or xml. Defaults to ascii.\n");
		printf("  -l, --list\t\t\tList known test cases\n");
		printf("  -t, --threads\t\t\tSpecify number of threads to run. Must be at least one, and at most the number of test cases linked\n");
		printf("  -v, --verbose\t\t\tIncrease the amount of output\n");
		printf("  -h, --help\t\t\tThis help screen\n");
		printf("\nFILTERS\n");
		printf("  If any arguments are passed that do not begin with -, they are considered filters.\n");
		printf("  If no filters are supplied, all test cases will be executed. Otherwise, only test cases\n");
		printf("  whose names match at least one filter will be executed.\n");
		return 0;
	}

	if (flags.listMode)
	{
		while (currentCase != NULL)
		{
			printf("%-30s* %s\n", currentCase->testCase->name, currentCase->testCase->description);
			currentCase = currentCase->next;
		}
	}

	// loop over test cases
	while (currentCase != NULL)
	{
		currentCase = currentCase->next;
	}

	// clean up
	potency_cleanup_test_cases();
	return 0;
}
