#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <potency.h>

char* buffer = NULL;

void* setup()
{
	buffer = calloc(1024, 1);
	return buffer;
}

void teardown(void* p)
{
	free(p);
	buffer = NULL;
}

POTENCY_SETUP(&setup);
POTENCY_TEARDOWN(&teardown);

TEST_CASE("/potency/samples/memcpy", "A simple example of a test case")
{
	REQUIRE(buffer);
	const char* str = "Hello, World\n";
	memcpy(buffer, str, strlen(str));
	CHECK(memcmp(buffer, str, strlen(str)) == 0);
}

TEST_CASE("/potency/samples/memset", "A simple example of a test case")
{
	REQUIRE(buffer);
	memset(buffer, 0, 1024);
	CHECK(strlen(buffer) == 0);
}

