#ifndef potency_report_ascii_h
#define potency_report_ascii_h

#include "potency.h"

extern void potency_print_report_header_ascii();
extern void potency_print_report_ascii();
extern void potency_print_report_footer_ascii();
extern void potency_print_assertion_ascii(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression);
extern void potency_print_requirement_ascii(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression);

#endif // !potency_report_ascii_h
