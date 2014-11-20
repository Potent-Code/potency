#include <stdio.h>
#include <string.h>
#include <potency.h>

TEST_CASE("/potency/samples/fprintf", "Test writing to a file")
{
	const char* filename = "myFile.txt";

	FILE* fh = fopen(filename, "w");
	// REQUIRE returns immediately if the condition is not true and does NOT generate an assertion count in the output
	// in this case we are not testing whether we can open the file, but we require that we can for the tests following
	REQUIRE(fh);

	// test writing to a file
	const char* str = "Hello, World!\n";
	const size_t len = strlen(str);
	CHECK(fprintf(fh, "%s", str) == (int)len);

	// test closing the file
	CHECK(fclose(fh) == 0);

	// to clean up, let's delete that file we just created
	CHECK(remove(filename) == 0);
}

