#include <potency.h>

TEST_CASE("longTestCase", "A test case with many assertions")
{
	uint32_t i = 0;
	for (i = 0; i < 10000000; i++)
	{
		CHECK(1==1);
	}
}

TEST_CASE("\\`*_{}[]()<>'#+-.!", "A test of markdown escaping")
{
	CHECK("\\`*_{}[]()<>'#+-.!" == 0);
}
