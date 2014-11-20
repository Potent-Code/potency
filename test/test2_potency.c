#include <potency.h>

TEST_CASE("/potency/testCase3", "Another test case")
{
	CHECK(1 != 0);
	CHECK(1 == 0);
}

void* potency_test_suite_setup()
{
	return NULL;
}

void potency_test_suite_teardown(void* unused)
{
	(void)unused;
	return;
}

POTENCY_SETUP(&potency_test_suite_setup);
POTENCY_TEARDOWN(&potency_test_suite_teardown);
