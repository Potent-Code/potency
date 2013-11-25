#ifndef potency_h
#define potency_h

#include <stdint.h>
#include <stdio.h>

// create a unique name by appending the line number to a name
#define potency_unique_name_line2( name, line ) name##line
#define potency_unique_name_line( name, line ) potency_unique_name_line2( name, line )
#define potency_unique_name( name ) potency_unique_name_line( name, __LINE__ )

#define potency_line_info3(file, line) file " (" #line "): "
#define potency_line_info2(file, line) potency_line_info3(file, line)
#define potency_line_info() potency_line_info2(__FILE__, __LINE__)

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

// list of potency_test_case structs
typedef struct _potency_test_case_list
{
	potency_test_case* testCase;
	struct _potency_test_case_list* next;
	struct _potency_test_case_list* prev;
} potency_test_case_list;

// potency_output_function takes __FILE__, __LINE__, #expression
typedef void(*potency_output_function)(potency_test_case* testCase, const char*, const uint32_t, const char*);

typedef void*(*potency_setup_function)();
typedef void(*potency_teardown_function)(void*);

extern void potency_add_setup_function(potency_setup_function _potency_setup);
extern void potency_add_teardown_function(potency_teardown_function _potency_teardown);
extern void potency_add_test_case( const char* name, const char* description, const potency_test_case_function _run );
extern potency_test_case_list* potency_get_test_case_list();
extern potency_output_function potency_print_assertion;
extern potency_output_function potency_print_requirement;

#define POTENCY_SETUP(function)\
	static void potency_unique_name( potency_setup_ )() __attribute__((constructor)); \
	static void potency_unique_name( potency_setup_ )()\
	{\
		potency_add_setup_function(function);\
	}\
	static int potency_unique_name( unused_int_ ) __attribute__((unused))

#define POTENCY_TEARDOWN(function)\
	static void potency_unique_name( potency_teardown_ )() __attribute__((constructor)); \
	static void potency_unique_name( potency_teardown_ )()\
	{\
		potency_add_teardown_function(function);\
	}\
	static int potency_unique_name( unused_int_ ) __attribute__((unused))

#define TEST_CASE( name, description )\
	static void potency_unique_name( potency_test_case_implementation_ )( potency_test_case* testCase ); \
	static void potency_unique_name( potency_test_case_ctor_ )()  __attribute__((constructor)); \
	static void potency_unique_name( potency_test_case_ctor_ )()\
	{\
		potency_add_test_case( name, description, &potency_unique_name( potency_test_case_implementation_ ));\
	} \
	static void potency_unique_name( potency_test_case_implementation_ )( potency_test_case* testCase )

#define CHECK(expression)\
	testCase->assertions++;\
	if (expression)\
	{\
		testCase->passedAssertions++;\
	}\
	else\
	{\
		(*potency_print_assertion)(testCase, __FILE__, __LINE__, #expression);\
	}

#define REQUIRE(expression)\
	if (!expression)\
	{\
		(*potency_print_requirement)(testCase, __FILE__, __LINE__, #expression);\
		return;\
	}

/*
#define CHECK(expression)\
	testCase->assertions++;\
	if (expression)\
	{\
		testCase->passedAssertions++;\
	}\
	else\
	{\
		printf("*** %s assertion failed: CHECK(%s)\n", potency_line_info(), #expression);\
	}

#define REQUIRE(expression)\
	if (!expression)\
	{\
		printf("!!! %s assertion failed: REQUIRE(%s)\n", potency_line_info(), #expression);\
		return;\
	}
*/

#endif // !potency_h
