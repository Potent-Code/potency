#ifndef potency_h
#define potency_h

// create a unique name by appending the line number to a name
#define potency_unique_name_line2( name, line ) name##line
#define potency_unique_name_line( name, line ) potency_unique_name_line2( name, line )
#define potency_unique_name( name ) potency_unique_name_line( name, __LINE__ )

// potency_test_case
typedef (void(*)()) potency_test_case_function;

// struct _potency_test_case is used to contain all data associated with a test case
typedef struct _potency_test_case
{
	const char* testCaseName;
	const char* testCaseDescription;
	potency_test_case_function runTestCase;
	size_t numberOfAssertions;
	size_t numberOfPassedAssertions;
	double timeToRun;
} potency_test_case;

#define TEST_CASE( name, description )\
static void potency_unique_name( potency_test_case_implementation_ )();\
potency_add_test_case( name, description, &potency_unique_name( potency_test_case_implementation_ ) ); \
static void potency_unique_name( potency_test_case_implmementation_ )()


#endif // !potency_h
