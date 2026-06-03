#include "parser.h"
#include "unity.h"
#include <string.h>

void setUp() {}
void tearDown() {}

void testParser(char *input, char *expected) {
    Lexer *l = newLexer(input);
    Parser *p = newParser(l);

    parser_parse(p);
    Error *errs = parser_errors(p);
    if (p->numErrors != 0) {
        fprintf(stderr, "Parser had some errors\n");

        for (int i = 0; i < p->numErrors; i++) {
            fprintf(stderr, "Parser Error: %s\n", errs[i].Value);
        }

        TEST_FAIL_MESSAGE("Parser had errors!");
    }
    AST *ast = parser_ast(p);

    char *result = ast_to_string(ast);

    TEST_ASSERT_EQUAL_STRING(expected, result);
}

void test_int_expression() {
    char *input = "1";
    char *expected = "1";
    testParser(input, expected);
}

void test_empty_input() {
    char *input = "";
    char *expected = "";
    testParser(input, expected);
}

void test_return_statement() {
    char *input = "(return 1)";
    char *expected = "(return 1)";
    testParser(input, expected);
}

void test_float_expression() {
    char *input = "1.5";
    char *expected = "1.5";
    testParser(input, expected);
}

void test_string_expression() {
    char *input = "\"hello\"";
    char *expected = "\"hello\"";
    testParser(input, expected);
}

void test_bool_expression() {
    char *input = "true";
    char *expected = "true";
    testParser(input, expected);
}

void test_bool_false_expression() {
    char *input = "false";
    char *expected = "false";
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

void test_identifier_expression() {
    char *input = "a";
    char *expected = "a";
    testParser(input, expected);
}

void test_nil_expression() {
    char *input = "nil";
    char *expected = "nil";
    testParser(input, expected);
}

void test_add() {
    char *input = "(+ 1 2)";
    char *expected = "(+ 1 2)";
    testParser(input, expected);
}

void test_subtract() {
    char *input = "(- 5 3)";
    char *expected = "(- 5 3)";
    testParser(input, expected);
}

void test_multiply() {
    char *input = "(* 2 3)";
    char *expected = "(* 2 3)";
    testParser(input, expected);
}

void test_divide() {
    char *input = "(/ 10 2)";
    char *expected = "(/ 10 2)";
    testParser(input, expected);
}

void test_modulo() {
    char *input = "(% 10 3)";
    char *expected = "(% 10 3)";
    testParser(input, expected);
}

void test_eq() {
    char *input = "(== 1 1)";
    char *expected = "(== 1 1)";
    testParser(input, expected);
}

void test_neq() {
    char *input = "(!= 1 2)";
    char *expected = "(!= 1 2)";
    testParser(input, expected);
}

void test_lt() {
    char *input = "(< 1 2)";
    char *expected = "(< 1 2)";
    testParser(input, expected);
}

void test_gt() {
    char *input = "(> 2 1)";
    char *expected = "(> 2 1)";
    testParser(input, expected);
}

void test_lte() {
    char *input = "(<= 1 1)";
    char *expected = "(<= 1 1)";
    testParser(input, expected);
}

void test_gte() {
    char *input = "(>= 2 1)";
    char *expected = "(>= 2 1)";
    testParser(input, expected);
}

void test_and() {
    char *input = "(and true false)";
    char *expected = "(and true false)";
    testParser(input, expected);
}

void test_or() {
    char *input = "(or true false)";
    char *expected = "(or true false)";
    testParser(input, expected);
}

void test_not() {
    char *input = "(not true)";
    char *expected = "(not true)";
    testParser(input, expected);
}

void test_string_concat() {
    char *input = "(.. \"hello\" \" world\")";
    char *expected = "(.. \"hello\" \" world\")";
    testParser(input, expected);
}

void test_return_with_call() {
    char *input = "(return (+ 1 2))";
    char *expected = "(return (+ 1 2))";
    testParser(input, expected);
}

void test_nested_call() {
    char *input = "(print (+ 1 2))";
    char *expected = "(print (+ 1 2))";
    testParser(input, expected);
}

void test_if_with_complex_condition() {
    char *input = "(if (== a 1) (print \"yes\"))";
    char *expected = "(if (== a 1) (print \"yes\"))";
    testParser(input, expected);
}

void test_let_with_call() {
    char *input = "(let a (+ 1 2))";
    char *expected = "(let a (+ 1 2))";
    testParser(input, expected);
}

void test_fn_with_single_param() {
    char *input = "(fn double (x) (* x 2))";
    char *expected = "(fn double (x) (* x 2))";
    testParser(input, expected);
}

void test_do_single() {
    char *input = "(do (print 1))";
    char *expected = "(do (print 1))";
    testParser(input, expected);
}

void test_do_multiple() {
    char *input = "(do (print 1) (print 2))";
    char *expected = "(do (print 1) (print 2))";
    testParser(input, expected);
}

void test_do_three() {
    char *input = "(do (print 1) (print 2) (print 3))";
    char *expected = "(do (print 1) (print 2) (print 3))";
    testParser(input, expected);
}

void test_anonymous_fn_no_params() {
    char *input = "(fn () 42)";
    char *expected = "(fn () 42)";
    testParser(input, expected);
}

void test_anonymous_fn_single_param() {
    char *input = "(fn (x) (* x 2))";
    char *expected = "(fn (x) (* x 2))";
    testParser(input, expected);
}

void test_anonymous_fn_two_params() {
    char *input = "(fn (x y) (+ x y))";
    char *expected = "(fn (x y) (+ x y))";
    testParser(input, expected);
}

void test_anonymous_fn_returning_fn() {
    char *input = "(fn (x) (fn (y) (+ x y)))";
    char *expected = "(fn (x) (fn (y) (+ x y)))";
    testParser(input, expected);
}

void test_do_in_if() {
    char *input = "(if true (do (print 1) (print 2)))";
    char *expected = "(if true (do (print 1) (print 2)))";
    testParser(input, expected);
}

void test_let_anonymous_fn() {
    char *input = "(let double (fn (x) (* x 2)))";
    char *expected = "(let double (fn (x) (* x 2)))";
    testParser(input, expected);
}

typedef void (*TestFn)(void);
typedef struct { const char *name; TestFn fn; } TestEntry;

static const TestEntry PARSER_TESTS[] = {
    // atoms
    {"test_empty_input",                  test_empty_input},
    {"test_int_expression",               test_int_expression},
    {"test_float_expression",             test_float_expression},
    {"test_string_expression",            test_string_expression},
    {"test_bool_expression",              test_bool_expression},
    {"test_bool_false_expression",        test_bool_false_expression},
    {"test_nil_expression",               test_nil_expression},
    {"test_identifier_expression",        test_identifier_expression},
    // arithmetic
    {"test_add",                          test_add},
    {"test_subtract",                     test_subtract},
    {"test_multiply",                     test_multiply},
    {"test_divide",                       test_divide},
    {"test_modulo",                       test_modulo},
    {"test_expression_statement_complex", test_expression_statement_complex},
    // comparison
    {"test_eq",                           test_eq},
    {"test_neq",                          test_neq},
    {"test_lt",                           test_lt},
    {"test_gt",                           test_gt},
    {"test_lte",                          test_lte},
    {"test_gte",                          test_gte},
    // boolean logic
    {"test_and",                          test_and},
    {"test_or",                           test_or},
    {"test_not",                          test_not},
    // string
    {"test_string_concat",                test_string_concat},
    // calls
    {"test_function_call",                test_function_call},
    {"test_nested_function_call",         test_nested_function_call},
    {"test_nested_call",                  test_nested_call},
    // return
    {"test_return_statement",             test_return_statement},
    {"test_return_with_call",             test_return_with_call},
    // let / set
    {"test_let_statement",                test_let_statement},
    {"test_let_statement_with_float",     test_let_statement_with_float},
    {"test_let_with_call",                test_let_with_call},
    {"test_let_anonymous_fn",             test_let_anonymous_fn},
    {"test_assignment_statement",         test_assignment_statement},
    // multiple statements
    {"test_expression_statement",         test_expression_statement},
    {"test_multiple_statements",          test_multiple_statements},
    {"test_multiple_let_statements",      test_multiple_let_statements},
    // if
    {"test_if_statement",                 test_if_statement},
    {"test_if_else_statement",            test_if_else_statement},
    {"test_if_with_complex_condition",    test_if_with_complex_condition},
    // do
    {"test_do_single",                    test_do_single},
    {"test_do_multiple",                  test_do_multiple},
    {"test_do_three",                     test_do_three},
    {"test_do_in_if",                     test_do_in_if},
    // fn
    {"test_function_statement",           test_function_statement},
    {"test_function_statement_with_args", test_function_statement_with_args},
    {"test_fn_with_single_param",         test_fn_with_single_param},
    {"test_anonymous_fn_no_params",       test_anonymous_fn_no_params},
    {"test_anonymous_fn_single_param",    test_anonymous_fn_single_param},
    {"test_anonymous_fn_two_params",      test_anonymous_fn_two_params},
    {"test_anonymous_fn_returning_fn",    test_anonymous_fn_returning_fn},
};

#define NUM_PARSER_TESTS (sizeof(PARSER_TESTS) / sizeof(PARSER_TESTS[0]))

int main(int argc, char *argv[]) {
    UNITY_BEGIN();
    if (argc >= 2) {
        for (size_t i = 0; i < NUM_PARSER_TESTS; i++) {
            if (strcmp(argv[1], PARSER_TESTS[i].name) == 0) {
                UnityDefaultTestRun(PARSER_TESTS[i].fn, PARSER_TESTS[i].name, 0);
                return UNITY_END();
            }
        }
        fprintf(stderr, "Unknown test: %s\n", argv[1]);
        return 1;
    }
    for (size_t i = 0; i < NUM_PARSER_TESTS; i++) {
        UnityDefaultTestRun(PARSER_TESTS[i].fn, PARSER_TESTS[i].name, 0);
    }
    return UNITY_END();
}
