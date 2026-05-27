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
    char *input = "return 1";

    char *expected = "return 1";

    testParser(input, expected);
}

void test_let_statement() {
    char *input = "let a int = 1";
    char *expected = "let a int = 1";
    testParser(input, expected);
}

void test_assignment_statement() {
    char *input = "a = 1";
    char *expected = "a = 1";
    testParser(input, expected);
}

void test_let_statement_with_float() {
    char *input = "let a int = 1.5";
    char *expected = "let a int = 1.5";
    testParser(input, expected);
}

void test_let_statement_with_type() {
    char *input = "let a int = 1";
    char *expected = "let a int = 1";
    testParser(input, expected);
}

void test_let_statement_multiple_with_type() {
    char *input = "let a int = 1 let b string = \"hello\" let c bool = true";
    char *expected = "let a int = 1 let b string = \"hello\" let c bool = true";
    testParser(input, expected);
}

void test_multiple_statements() {
    char *input = "let a int = 1 let b int = 2";
    char *expected = "let a int = 1 let b int = 2";
    testParser(input, expected);
}

void test_let_statement_without_type() {
    char *input = "let a = 1";
    char *expected = "let a auto = 1";
    testParser(input, expected);
}

void test_if_statement() {
    char *input = "if true then print(\"true\") end";
    char *expected = "if true then print(\"true\") end";
    testParser(input, expected);
}

void test_if_else_statement() {
    char *input = "if true then print(\"true\") else print(\"false\") end";
    char *expected = "if true then print(\"true\") else print(\"false\") end";
    testParser(input, expected);
}

void test_return_statement() {
    char *input = "return 1";
    char *expected = "return 1";
    testParser(input, expected);
}

void test_expression_statement() {
    char *input = "1";
    char *expected = "1";
    testParser(input, expected);
}

void test_expression_statement_complex() {
    char *input = "1 + 2 * 3";
    char *expected = "(1 + (2 * 3))";
    testParser(input, expected);
}

void test_function_statement() {
    char *input = "fn hello() void then print(\"Hello, World\") end";
    char *expected = "fn hello() void then print(\"Hello, World\") end";
    testParser(input, expected);
}

void test_function_statement_with_args() {
    char *input = "fn hello(a int, b int) int then return a + b end";
    char *expected = "fn hello(a int, b int) int then return (a + b) end";
    testParser(input, expected);
}

void test_function_statement_with_return_type() {
    char *input =
        "fn concat(a string, b string) string then return (a + b) end";
    char *expected =
        "fn concat(a string, b string) string then return (a + b) end";
    testParser(input, expected);
}

void test_function_statement_with_function_arg_type() {
    char *input =
        "fn apply(x int, somefunc fn(int) int) int then return somefunc(x) end";
    char *expected =
        "fn apply(x int, somefunc fn(int) int) int then return somefunc(x) end";
    testParser(input, expected);
}

void test_function_statement_with_function_return_type() {
    char *input =
        "fn adder(x int, y int) fn(int) int then return \"something\" end";
    char *expected =
        "fn adder(x int, y int) fn(int) int then return \"something\" end";
    testParser(input, expected);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_simple);
    return UNITY_END();
}
