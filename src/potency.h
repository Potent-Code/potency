#ifndef potency_h
#define potency_h

#include <stdint.h>
#include <stdio.h>

// create a unique name by appending the line number to a name
#define potency_unique_name_line2( name, line ) name##line
#define potency_unique_name_line( name, line ) potency_unique_name_line2( name, line )
#define potency_unique_name( name ) potency_unique_name_line( name, __LINE__ )

// potency_test_case
struct _potency_test_case;
typedef void(*potency_test_case_function)( struct _potency_test_case* );

// struct _potency_test_case is used to contain all data associated with a test case
typedef struct _potency_test_case
{
	const char* name;
	const char* description;
	potency_test_case_function run;
	uint32_t assertions;
	uint32_t passedAssertions;
	double runTime;
} potency_test_case;

// static int potency_add_test_case( const char* name, const char* description, const potency_test_case_function _run );

#define TEST_CASE( name, description )\
static void potency_unique_name( potency_test_case_implementation_ )(potency_test_case* testCase); \
static void potency_unique_name( potency_test_case_implementation_ )(potency_test_case* testCase)
//static const int potency_unique_name( potency_unused_ ) = potency_add_test_case( name, description, &potency_unique_name(potency_test_case_implementation_)); 

#define CHECK(expression)\
	testCase->assertions++;\
	if (expression)\
	{\
		testCase->passedAssertions++;\
	}\
	else\
	{\
		printf("*** assertion failed: %s\n", #expression);\
	}


#endif // !potency_h
