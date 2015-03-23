#ifndef potency_report_json_h
#define potency_report_json_h

#include "potency.h"

extern void potency_print_report_header_json(const char* testSuite);
extern void potency_print_report_json();
extern void potency_print_report_footer_json();
extern void potency_print_assertion_json(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);
extern void potency_print_requirement_json(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);
extern void potency_print_list_json();

#endif // !potency_report_json_h

