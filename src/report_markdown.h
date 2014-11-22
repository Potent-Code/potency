#ifndef potency_report_markdown_h
#define potency_report_markdown_h

extern void potency_print_report_header_markdown(const char* testSuite);
extern void potency_print_report_markdown();
extern void potency_print_report_footer_markdown();
extern void potency_print_assertion_markdown(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);
extern void potency_print_requirement_markdown(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);

#endif // !potency_report_markdown_h

