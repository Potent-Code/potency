#include <stdint.h>
#include <potency.h>

TEST_CASE("/potency/internal/check", "A test of the CHECK macro")
{
	CHECK(1 == 1);
	CHECK(0 == 0);
	CHECK(NULL == 0);
}

TEST_CASE("/potency/internal/require", "A test of the REQUIRE macro")
{
	REQUIRE(1 == 1);
	REQUIRE(0 == 0);
	REQUIRE(NULL == 0);
}

TEST_CASE("/potency/internal/longTestCase", "A test case with many assertions")
{
	uint16_t i = 0;
	for (i = 0; i < UINT16_MAX; i++)
	{
		CHECK(1==1);
	}
}

TEST_CASE("/potency/internal/\\`*_{}[]()<>'#+-.!", "A test of markdown escaping")
{
	// the following test case intentionally fails so we can see the escaped output
	CHECK("\\`*_{}[]()<>'#+-.!" == 0);
}

