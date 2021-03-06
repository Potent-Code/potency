#ifndef potency_h
#define potency_h

#include <stdbool.h>
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

// output mode
typedef enum
{
	outputFormatInvalid = -1,
	outputFormatMarkdown,
	outputFormatJSON,
	outputFormatXML
} output_format;

// run mode settings, usually given by command line arguments
typedef struct _potency_settings
{
	size_t threads;			// number of threads to run test cases
	output_format format;	// what format to generate report in
	const char* outputFile;	// file to save report to
	bool verboseMode;		// print extra output
	bool listMode;			// should we just list test cases and exit?
	bool helpMode;			// print the help screen
	bool filteringOn;		// true if we find any filters
} potency_settings;

extern potency_settings potencySettings;

// struct _potency_test_case is used to contain all data associated with a test case
typedef struct _potency_test_case
{
	const char* name;
	const char* description;
	potency_test_case_function run;
	bool runTestCase;
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
typedef void(*potency_output_function)(potency_test_case* testCase, bool, const char*, const uint32_t, const char*);

typedef void*(*potency_setup_function)();
typedef void(*potency_teardown_function)(void*);

extern void potency_add_setup_function(potency_setup_function _potency_setup);
extern void potency_add_teardown_function(potency_teardown_function _potency_teardown);
extern void potency_add_test_case( const char* name, const char* description, const potency_test_case_function _run );
extern potency_test_case_list* potency_get_test_case_list();
extern potency_output_function potency_print_assertion;
extern potency_output_function potency_print_requirement;

extern FILE* reportFileHandle;

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
		if (potencySettings.verboseMode)\
		{\
			(*potency_print_assertion)(testCase, true, __FILE__, __LINE__, #expression);\
		}\
	}\
	else\
	{\
		(*potency_print_assertion)(testCase, false, __FILE__, __LINE__, #expression);\
	}

#define REQUIRE(expression)\
	if (!expression)\
	{\
		(*potency_print_requirement)(testCase, false, __FILE__, __LINE__, #expression);\
		return;\
	}\
	else if (potencySettings.verboseMode)\
	{\
		(*potency_print_requirement)(testCase, true, __FILE__, __LINE__, #expression);\
	}

#endif // !potency_h

