#include "potency.h"

#include <stdlib.h>

typedef struct _potency_test_case_list
{
	potency_test_case* testCase;
	struct _potency_test_case_list* next;
	struct _potency_test_case_list* prev;
} potency_test_case_list;

static potency_test_case_list* testCaseHead = NULL;

// static int potency_add_test_case( const char* name, const char* description, potency_test_case_function _run )
// {
// 	potency_test_case* newTestCase = calloc(1, sizeof(*newTestCase));
// 	newTestCase->name = name;
// 	newTestCase->description = description;
// 	newTestCase->run = _run;

// 	if (testCaseHead == NULL)
// 	{
// 		testCaseHead = calloc(1, sizeof(*testCaseHead));
// 		testCaseHead->testCase = newTestCase;
// 		return 0;
// 	}

// 	potency_test_case_list* testCaseEnd = testCaseHead;
// 	while (testCaseEnd->next != NULL)
// 	{
// 		testCaseEnd = testCaseEnd->next;
// 	}

// 	potency_test_case_list* newTestCaseList = calloc(1, sizeof(*newTestCaseList));
// 	newTestCaseList->testCase = newTestCase;
// 	newTestCaseList->prev = testCaseEnd;
// 	testCaseEnd->next = newTestCaseList;

// 	return 0;
// }

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
	(void)argc;
	(void)argv;
	potency_cleanup_test_cases();
	return 0;
}

TEST_CASE("exampleTestCase", "Just here for testing test cases")
{
	CHECK(1 == 1);
}
