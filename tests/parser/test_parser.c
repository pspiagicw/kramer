#include "parser.h"
#include "unity.h"

void setUp() {}
void tearDown() {}

void testParser(char *input, char *expected) {
    Lexer *l = newLexer(input);
    Parser *p = newParser(l);

    AST *ast = parser_parse(p);

    char *result = ast_to_string(ast);

    TEST_ASSERT_EQUAL_STRING(expected, result);
}

void test_simple() {
    char *input = "(return 1)";

    char *expected = "(return 1)";

    testParser(input, expected);
}

void test_let_statement() {
    char *input = "(let a 1)";
    char *expected = "(let a 1)";
    testParser(input, expected);
}

void test_assignment_statement() {
    char *input = "(set a 1)";
    char *expected = "(set a 1)";
    testParser(input, expected);
}

void test_let_statement_with_float() {
    char *input = "(let a 1.5)";
    char *expected = "(let a 1.5)";
    testParser(input, expected);
}

void test_multiple_statements() {
    char *input = "(let a 1) (let b 2)";
    char *expected = "(let a 1) (let b 2)";
    testParser(input, expected);
}

void test_multiple_let_statements() {
    char *input = "(let a 1) (let b \"hello\") (let c true)";
    char *expected = "(let a 1) (let b \"hello\") (let c true)";
    testParser(input, expected);
}

void test_if_statement() {
    char *input = "(if true (print \"true\"))";
    char *expected = "(if true (print \"true\"))";
    testParser(input, expected);
}

void test_if_else_statement() {
    char *input = "(if true (print \"true\") (print \"false\"))";
    char *expected = "(if true (print \"true\") (print \"false\"))";
    testParser(input, expected);
}

void test_return_statement() {
    char *input = "(return 1)";
    char *expected = "(return 1)";
    testParser(input, expected);
}

void test_expression_statement() {
    char *input = "1";
    char *expected = "1";
    testParser(input, expected);
}

void test_expression_statement_complex() {
    char *input = "(+ 1 (* 2 3))";
    char *expected = "(+ 1 (* 2 3))";
    testParser(input, expected);
}

void test_function_statement() {
    char *input = "(fn hello () (print \"Hello, World\"))";
    char *expected = "(fn hello () (print \"Hello, World\"))";
    testParser(input, expected);
}

void test_function_statement_with_args() {
    char *input = "(fn hello (a b) (return (+ a b)))";
    char *expected = "(fn hello (a b) (return (+ a b)))";
    testParser(input, expected);
}

void test_function_call() {
    char *input = "(add 1 2)";
    char *expected = "(add 1 2)";
    testParser(input, expected);
}

void test_nested_function_call() {
    char *input = "(apply somefunc x)";
    char *expected = "(apply somefunc x)";
    testParser(input, expected);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_simple);
    return UNITY_END();
}
