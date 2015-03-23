#ifndef potency_report_xml_h
#define potency_report_xml_h

extern void potency_print_report_header_xml(const char* testSuite);
extern void potency_print_report_xml();
extern void potency_print_report_footer_xml();
extern void potency_print_assertion_xml(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);
extern void potency_print_requirement_xml(potency_test_case* testCase, bool passed, const char* file, const uint32_t line, const char* expression);
extern void potency_print_list_xml();

#endif // !potency_report_xml_h

