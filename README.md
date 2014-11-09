# Potency #

Potency is a unit test framework for C that makes it easy to write unit tests.
Potency supports test case filtering, benchmarking, and custom set up and tear down functions.
Potency has output modes for Markdown, XML, and JSON so your unit tests can easily parsed by continuous integration systems.

## Dependencies ##
Potency depends on bam to build. If you don't have it already, you can get it at http://matricks.github.io/bam/
Bam is an excellent build system that is extremely portable, lightweight, fast, and flexible.

## Installation ##
To install potency, simply run the following commands:

```
./make.sh
./make.sh install
```

Potency will build and install with a base path of /usr/local/lib/

## Usage ##
Let's have a look at how easy Potency makes writing unit tests with some examples.

```C
#include <stdio.h>
#include <string.h>
#include <potency.h>
#include "myHeader.h"

TEST_CASE("samples/testCase1", "A simple example of a test case")
{
	// test a function that returns a known value
	// CHECK generates an assertion count in the output
	CHECK(myFunc(5) == 3);

	FILE* fh = fopen("myFile.txt", "w");
	// REQUIRE returns immediately if the condition is not true and does NOT generate an assertion count in the output
	// in this case we are not testing whether we can open the file, but we require that we can for the tests following
	REQUIRE(fh != NULL);

	// test writing to a file
	const char* str = "Hello, World!\n";
	const size_t len = strlen(str);
	CHECK(fprintf(fh, "%s", str) == len);
}

```

That's it! So we have seen how we can write self contained test cases with the TEST_CASE(), CHECK(), and REQUIRE() macros. A collection 
of test cases can be compiled together and linked to potency to form a test suite. But what if we want to write test cases that are 
not self contained? Potency provides the ability to add a setup or a teardown function. Here's an example in which we use this to share 
a buffer across test cases without leaking memory.

```C
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

TEST_CASE("samples/testCase2", "A simple example of a test case")
{
	REQUIRE(buffer != NULL);
	const char* str = "Hello, World\n";
	memcpy(buffer, str, strlen(str));
	CHECK(memcmp(buffer, str, strlen(str)) == 0);
}

TEST_CASE("samples/testCase3", "A simple example of a test case")
{
	REQUIRE(buffer != NULL);
	memset(buffer, 0, 1024);
	CHECK(strlen(buffer) == 0);	
}

```

## Contact ##
If you have any questions regarding Potency or its usage, please feel free to e-mail me at ryan.lucchese@write-c.com 
