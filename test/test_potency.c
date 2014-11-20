#include <potency.h>

TEST_CASE("/potency/exampleTestCase", "Just here for testing test cases")
{
	CHECK(1 == 1);
}

TEST_CASE("/potency/example2", "Another test case test")
{
	CHECK(0 == 0);
	CHECK(NULL == 0);
}
