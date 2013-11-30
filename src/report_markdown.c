#include "potency.h"
#include "report.h"
#include "report_markdown.h"

#include <time.h>
#include <string.h>

static potency_test_case* currentTestCase = NULL;

char* potency_escape_markdown(const char* unescaped, char* escaped, size_t escapedLength)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; (i < strlen(unescaped)) && (j < (escapedLength - 1)); i++)
	{
		// escape codes at http://daringfireball.net/projects/markdown/syntax#backslash
		switch(unescaped[i])
		{
			case '\\':
			case '`':
			case '*':
			case '_':
			case '{':
			case '}':
			case '[':
			case ']':
			case '(':
			case ')':
			case '#':
			case '+':
			case '-':
			case '.':
			case '!':
				escaped[j++] = '\\';
				escaped[j++] = unescaped[i];
				break;
			default:
				escaped[j++] = unescaped[i];
		}
	}

	// null terminate
	escaped[j] = 0;

	return escaped;
}

// special function for escaping markdown only inside of backticks
char* potency_escape_markdown_backticks(const char* unescaped, char* escaped, size_t escapedLength)
{
	size_t i = 0;
	size_t j = 0;

	for (i = 0; (i < strlen(unescaped)) && (j < (escapedLength - 1)); i++)
	{
		// escape codes at http://daringfireball.net/projects/markdown/syntax#backslash
		switch(unescaped[i])
		{
			case '\\':
				escaped[j++] = '\\';
				escaped[j++] = unescaped[i];
				break;
			case '`': // escape '`' with " ` `` ` `` ` "
				escaped[j++] = ' ';
				escaped[j++] = '`';
				escaped[j++] = ' ';
				escaped[j++] = '`';
				escaped[j++] = '`';
				escaped[j++] = ' ';
				escaped[j++] = '`';
				escaped[j++] = ' ';
				escaped[j++] = '`';
				escaped[j++] = '`';
				escaped[j++] = ' ';
				escaped[j++] = '`';
				escaped[j++] = ' ';
				break;
			default:
				escaped[j++] = unescaped[i];
		}
	}

	// null terminate
	escaped[j] = 0;

	return &escaped[0];
}

void potency_print_report_header_markdown(const char* testSuite)
{
	time_t currentTime = time(NULL);
	const char* currentTimeString = ctime(&currentTime);
	const size_t escapedMarkdownLength = 4096;
	char escapedMarkdown[escapedMarkdownLength];

	fprintf(reportFileHandle, "# %s #\n", potency_escape_markdown(testSuite, escapedMarkdown, escapedMarkdownLength));
	fprintf(reportFileHandle, "> %.*s  \n  \n", (int)strlen(currentTimeString) - 1, potency_escape_markdown(currentTimeString, escapedMarkdown, escapedMarkdownLength));
}

void potency_print_report_markdown()
{
	test_statistics stats;
	potency_collect_statistics(&stats);

	fprintf(reportFileHandle, "\n## Unit Test Report ##\n");
	fprintf(reportFileHandle, "> *  Success Percentage: %.2f%%\n", stats.successPercentage);
	fprintf(reportFileHandle, "> *  Passed Assertions: %u\n", stats.passedAssertions);
	fprintf(reportFileHandle, "> *  Failed Assertions: %u\n", stats.failedAssertions);
	fprintf(reportFileHandle, "> *  Total Assertions: %u\n", stats.assertions);
	fprintf(reportFileHandle, "> *  Test Cases: %u/%u\n", stats.testCasesRan, stats.testCases);
}

void potency_print_report_footer_markdown()
{
	fprintf(reportFileHandle, "\n");
}

void potency_print_test_case_markdown(potency_test_case* testCase)
{
	if (currentTestCase != testCase)
	{
		currentTestCase = testCase;
		const size_t escapedMarkdownLength = 4096;
		char escapedMarkdown[escapedMarkdownLength];

		fprintf(reportFileHandle, "\n### %s: ###\n", potency_escape_markdown(currentTestCase->name, escapedMarkdown, escapedMarkdownLength));
	}
}

void potency_print_assertion_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	const size_t escapedMarkdownLength = 4096;
	char escapedMarkdownFile[escapedMarkdownLength];
	char escapedMarkdownExpression[escapedMarkdownLength];

	fprintf(reportFileHandle, "> 1. **%s (%u)**: *assertion failed* - ` CHECK(%s) `  \n", potency_escape_markdown(file, escapedMarkdownFile, escapedMarkdownLength), line, potency_escape_markdown_backticks(expression, escapedMarkdownExpression, escapedMarkdownLength));
}

void potency_print_requirement_markdown(potency_test_case* testCase, const char* file, const uint32_t line, const char* expression)
{
	potency_print_test_case_markdown(testCase);
	const size_t escapedMarkdownLength = 4096;
	char escapedMarkdownFile[escapedMarkdownLength];
	char escapedMarkdownExpression[escapedMarkdownLength];

	fprintf(reportFileHandle, "> 1. **%s (%u)**: *assertion failed* - ` REQUIRE(%s) `  \n", potency_escape_markdown(file, escapedMarkdownFile, escapedMarkdownLength), line, potency_escape_markdown_backticks(expression, escapedMarkdownExpression, escapedMarkdownLength));
}
