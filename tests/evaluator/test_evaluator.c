#include "eval.h"
#include "parser.h"
#include "unity.h"
#include <string.h>

void setUp() {}
void tearDown() {}

Value *testEval(char *input) {
    Lexer *l = newLexer(input);
    Parser *p = newParser(l);
    parser_parse(p);
    if (p->numErrors != 0) {
        Error *errs = parser_errors(p);
        for (int i = 0; i < p->numErrors; i++) {
            fprintf(stderr, "Parser Error: %s\n", errs[i].Value);
        }
        TEST_FAIL_MESSAGE("Parser had errors!");
    }
    Environment *env = env_new(NULL);
    return eval_ast(parser_ast(p), env);
}

void assertEval(char *input, char *expected) {
    Value *result = testEval(input);
    TEST_ASSERT_EQUAL_STRING(expected, value_to_string(result));
}

// atoms
void test_eval_integer() { assertEval("5", "5"); }
void test_eval_float() { assertEval("3.14", "3.14"); }
void test_eval_bool_true() { assertEval("true", "true"); }
void test_eval_bool_false() { assertEval("false", "false"); }
void test_eval_nil() { assertEval("nil", "nil"); }
void test_eval_string() { assertEval("\"hello\"", "hello"); }

// arithmetic
void test_eval_add() { assertEval("(+ 2 3)", "5"); }
void test_eval_subtract() { assertEval("(- 10 4)", "6"); }
void test_eval_multiply() { assertEval("(* 3 4)", "12"); }
void test_eval_divide() { assertEval("(/ 10 2)", "5"); }
void test_eval_modulo() { assertEval("(% 10 3)", "1"); }
void test_eval_nested() { assertEval("(+ (* 2 3) 4)", "10"); }

// addition: single arg (identity), int+int, float+int, float+float
void test_eval_add_single_int() { assertEval("(+ 5)", "5"); }
void test_eval_add_single_float() { assertEval("(+ 2.5)", "2.5"); }
void test_eval_add_int_int() { assertEval("(+ 2 3)", "5"); }
void test_eval_add_float_int() { assertEval("(+ 2.5 3)", "5.5"); }
void test_eval_add_float_float() { assertEval("(+ 2.5 1.25)", "3.75"); }

// subtraction: single arg (negation), int-int, float-int, float-float
void test_eval_subtract_single_int() { assertEval("(- 5)", "-5"); }
void test_eval_subtract_single_float() { assertEval("(- 2.5)", "-2.5"); }
void test_eval_subtract_int_int() { assertEval("(- 10 4)", "6"); }
void test_eval_subtract_float_int() { assertEval("(- 5.5 2)", "3.5"); }
void test_eval_subtract_float_float() { assertEval("(- 5.5 1.25)", "4.25"); }

// multiplication: single arg (identity), int*int, float*int, float*float
void test_eval_multiply_single_int() { assertEval("(* 5)", "5"); }
void test_eval_multiply_single_float() { assertEval("(* 2.5)", "2.5"); }
void test_eval_multiply_int_int() { assertEval("(* 3 4)", "12"); }
void test_eval_multiply_float_int() { assertEval("(* 2.5 3)", "7.5"); }
void test_eval_multiply_float_float() { assertEval("(* 2.5 1.5)", "3.75"); }

// division: single arg (reciprocal), int/int, float/int, float/float
void test_eval_divide_single_int() { assertEval("(/ 5)", "0.2"); }
void test_eval_divide_single_float() { assertEval("(/ 2.0)", "0.5"); }
void test_eval_divide_int_int() { assertEval("(/ 10 2)", "5"); }
void test_eval_divide_float_int() { assertEval("(/ 7.5 3)", "2.5"); }
void test_eval_divide_float_float() { assertEval("(/ 9.0 4.0)", "2.25"); }

// comparison
void test_eval_eq_true() { assertEval("(== 1 1)", "true"); }
void test_eval_eq_false() { assertEval("(== 1 2)", "false"); }
void test_eval_eq_string_true() { assertEval("(== \"abc\" \"abc\")", "true"); }
void test_eval_eq_string_false() {
    assertEval("(== \"abc\" \"xyz\")", "false");
}
void test_eval_eq_bool_true() { assertEval("(== true true)", "true"); }
void test_eval_eq_bool_false() { assertEval("(== true false)", "false"); }
void test_eval_eq_float_true() { assertEval("(== 1.5 1.5)", "true"); }
void test_eval_eq_float_false() { assertEval("(== 1.5 2.5)", "false"); }
void test_eval_neq_int_true() { assertEval("(!= 1 2)", "true"); }
void test_eval_neq_int_false() { assertEval("(!= 1 1)", "false"); }
void test_eval_neq_string_true() { assertEval("(!= \"abc\" \"xyz\")", "true"); }
void test_eval_neq_string_false() {
    assertEval("(!= \"abc\" \"abc\")", "false");
}
void test_eval_neq_bool_true() { assertEval("(!= true false)", "true"); }
void test_eval_neq_bool_false() { assertEval("(!= true true)", "false"); }
void test_eval_neq_float_true() { assertEval("(!= 1.5 2.5)", "true"); }
void test_eval_neq_float_false() { assertEval("(!= 1.5 1.5)", "false"); }
void test_eval_lt() { assertEval("(< 1 2)", "true"); }
void test_eval_gt() { assertEval("(> 2 1)", "true"); }
void test_eval_lte() { assertEval("(<= 1 2)", "true"); }
void test_eval_lte_equal() { assertEval("(<= 2 2)", "true"); }
void test_eval_gte() { assertEval("(>= 2 1)", "true"); }
void test_eval_gte_equal() { assertEval("(>= 2 2)", "true"); }

// variadic comparison: lt (<), strictly increasing
void test_eval_lt_variadic_int() { assertEval("(< 1 2 3)", "true"); }
void test_eval_lt_variadic_int_false() { assertEval("(< 1 3 2)", "false"); }
void test_eval_lt_variadic_float() { assertEval("(< 1.5 2.5 3.5)", "true"); }
void test_eval_lt_variadic_mixed() { assertEval("(< 1 2.5 3)", "true"); }

// variadic comparison: gt (>), strictly decreasing
void test_eval_gt_variadic_int() { assertEval("(> 3 2 1)", "true"); }
void test_eval_gt_variadic_int_false() { assertEval("(> 3 1 2)", "false"); }
void test_eval_gt_variadic_float() { assertEval("(> 3.5 2.5 1.5)", "true"); }
void test_eval_gt_variadic_mixed() { assertEval("(> 3 2.5 1)", "true"); }

// variadic comparison: lte (<=), non-decreasing
void test_eval_lte_variadic_int() { assertEval("(<= 1 2 2)", "true"); }
void test_eval_lte_variadic_int_false() { assertEval("(<= 1 3 2)", "false"); }
void test_eval_lte_variadic_float() { assertEval("(<= 1.5 2.5 2.5)", "true"); }
void test_eval_lte_variadic_mixed() { assertEval("(<= 1 2.0 2.5)", "true"); }

// variadic comparison: gte (>=), non-increasing
void test_eval_gte_variadic_int() { assertEval("(>= 3 2 2)", "true"); }
void test_eval_gte_variadic_int_false() { assertEval("(>= 3 1 2)", "false"); }
void test_eval_gte_variadic_float() { assertEval("(>= 3.5 2.5 2.5)", "true"); }
void test_eval_gte_variadic_mixed() { assertEval("(>= 3.0 2 2.0)", "true"); }

// boolean logic
void test_eval_and() { assertEval("(and true false)", "false"); }
void test_eval_or() { assertEval("(or true false)", "true"); }
void test_eval_not() { assertEval("(not true)", "false"); }

// variadic and: true only when every operand is true
void test_eval_and_variadic_true() {
    assertEval("(and true true true)", "true");
}
void test_eval_and_variadic_false() {
    assertEval("(and true true false)", "false");
}

// variadic or: true when any operand is true
void test_eval_or_variadic_true() {
    assertEval("(or false false true)", "true");
}
void test_eval_or_variadic_false() {
    assertEval("(or false false false)", "false");
}

// string
void test_eval_concat() {
    assertEval("(.. \"hello\" \" world\")", "hello world");
}

// let
void test_eval_let() { assertEval("(let x 5) x", "5"); }
void test_eval_let_expr() { assertEval("(let x (+ 2 3)) x", "5"); }

// if
void test_eval_if_true() { assertEval("(if true 1 2)", "1"); }
void test_eval_if_false() { assertEval("(if false 1 2)", "2"); }
void test_eval_if_no_else() { assertEval("(if false 1)", "nil"); }

// fn
void test_eval_fn_call() {
    assertEval("(fn add (x y) (+ x y)) (add 3 4)", "7");
}
void test_eval_return() { assertEval("(fn id (x) (return x)) (id 42)", "42"); }

typedef void (*TestFn)(void);
typedef struct {
    const char *name;
    TestFn fn;
} TestEntry;

static const TestEntry EVALUATOR_TESTS[] = {
    // atoms
    {"test_eval_integer", test_eval_integer},
    {"test_eval_float", test_eval_float},
    {"test_eval_bool_true", test_eval_bool_true},
    {"test_eval_bool_false", test_eval_bool_false},
    {"test_eval_nil", test_eval_nil},
    {"test_eval_string", test_eval_string},
    // arithmetic
    {"test_eval_add", test_eval_add},
    {"test_eval_subtract", test_eval_subtract},
    {"test_eval_multiply", test_eval_multiply},
    {"test_eval_divide", test_eval_divide},
    {"test_eval_modulo", test_eval_modulo},
    {"test_eval_nested", test_eval_nested},
    // addition
    {"test_eval_add_single_int", test_eval_add_single_int},
    {"test_eval_add_single_float", test_eval_add_single_float},
    {"test_eval_add_int_int", test_eval_add_int_int},
    {"test_eval_add_float_int", test_eval_add_float_int},
    {"test_eval_add_float_float", test_eval_add_float_float},
    // subtraction
    {"test_eval_subtract_single_int", test_eval_subtract_single_int},
    {"test_eval_subtract_single_float", test_eval_subtract_single_float},
    {"test_eval_subtract_int_int", test_eval_subtract_int_int},
    {"test_eval_subtract_float_int", test_eval_subtract_float_int},
    {"test_eval_subtract_float_float", test_eval_subtract_float_float},
    // multiplication
    {"test_eval_multiply_single_int", test_eval_multiply_single_int},
    {"test_eval_multiply_single_float", test_eval_multiply_single_float},
    {"test_eval_multiply_int_int", test_eval_multiply_int_int},
    {"test_eval_multiply_float_int", test_eval_multiply_float_int},
    {"test_eval_multiply_float_float", test_eval_multiply_float_float},
    // division
    {"test_eval_divide_single_int", test_eval_divide_single_int},
    {"test_eval_divide_single_float", test_eval_divide_single_float},
    {"test_eval_divide_int_int", test_eval_divide_int_int},
    {"test_eval_divide_float_int", test_eval_divide_float_int},
    {"test_eval_divide_float_float", test_eval_divide_float_float},
    // comparison
    {"test_eval_eq_true", test_eval_eq_true},
    {"test_eval_eq_false", test_eval_eq_false},
    {"test_eval_eq_string_true", test_eval_eq_string_true},
    {"test_eval_eq_string_false", test_eval_eq_string_false},
    {"test_eval_eq_bool_true", test_eval_eq_bool_true},
    {"test_eval_eq_bool_false", test_eval_eq_bool_false},
    {"test_eval_eq_float_true", test_eval_eq_float_true},
    {"test_eval_eq_float_false", test_eval_eq_float_false},
    {"test_eval_neq_int_true", test_eval_neq_int_true},
    {"test_eval_neq_int_false", test_eval_neq_int_false},
    {"test_eval_neq_string_true", test_eval_neq_string_true},
    {"test_eval_neq_string_false", test_eval_neq_string_false},
    {"test_eval_neq_bool_true", test_eval_neq_bool_true},
    {"test_eval_neq_bool_false", test_eval_neq_bool_false},
    {"test_eval_neq_float_true", test_eval_neq_float_true},
    {"test_eval_neq_float_false", test_eval_neq_float_false},
    {"test_eval_lt", test_eval_lt},
    {"test_eval_gt", test_eval_gt},
    {"test_eval_lte", test_eval_lte},
    {"test_eval_lte_equal", test_eval_lte_equal},
    {"test_eval_gte", test_eval_gte},
    {"test_eval_gte_equal", test_eval_gte_equal},
    // variadic comparison
    {"test_eval_lt_variadic_int", test_eval_lt_variadic_int},
    {"test_eval_lt_variadic_int_false", test_eval_lt_variadic_int_false},
    {"test_eval_lt_variadic_float", test_eval_lt_variadic_float},
    {"test_eval_lt_variadic_mixed", test_eval_lt_variadic_mixed},
    {"test_eval_gt_variadic_int", test_eval_gt_variadic_int},
    {"test_eval_gt_variadic_int_false", test_eval_gt_variadic_int_false},
    {"test_eval_gt_variadic_float", test_eval_gt_variadic_float},
    {"test_eval_gt_variadic_mixed", test_eval_gt_variadic_mixed},
    {"test_eval_lte_variadic_int", test_eval_lte_variadic_int},
    {"test_eval_lte_variadic_int_false", test_eval_lte_variadic_int_false},
    {"test_eval_lte_variadic_float", test_eval_lte_variadic_float},
    {"test_eval_lte_variadic_mixed", test_eval_lte_variadic_mixed},
    {"test_eval_gte_variadic_int", test_eval_gte_variadic_int},
    {"test_eval_gte_variadic_int_false", test_eval_gte_variadic_int_false},
    {"test_eval_gte_variadic_float", test_eval_gte_variadic_float},
    {"test_eval_gte_variadic_mixed", test_eval_gte_variadic_mixed},
    // boolean logic
    {"test_eval_and", test_eval_and},
    {"test_eval_or", test_eval_or},
    {"test_eval_and_variadic_true", test_eval_and_variadic_true},
    {"test_eval_and_variadic_false", test_eval_and_variadic_false},
    {"test_eval_or_variadic_true", test_eval_or_variadic_true},
    {"test_eval_or_variadic_false", test_eval_or_variadic_false},
    {"test_eval_not", test_eval_not},
    // string
    {"test_eval_concat", test_eval_concat},
    // let
    {"test_eval_let", test_eval_let},
    {"test_eval_let_expr", test_eval_let_expr},
    // if
    {"test_eval_if_true", test_eval_if_true},
    {"test_eval_if_false", test_eval_if_false},
    {"test_eval_if_no_else", test_eval_if_no_else},
    // fn
    {"test_eval_fn_call", test_eval_fn_call},
    {"test_eval_return", test_eval_return},
};

#define NUM_EVALUATOR_TESTS                                                    \
    (sizeof(EVALUATOR_TESTS) / sizeof(EVALUATOR_TESTS[0]))

int main(int argc, char *argv[]) {
    UNITY_BEGIN();
    if (argc >= 2) {
        for (size_t i = 0; i < NUM_EVALUATOR_TESTS; i++) {
            if (strcmp(argv[1], EVALUATOR_TESTS[i].name) == 0) {
                UnityDefaultTestRun(EVALUATOR_TESTS[i].fn,
                                    EVALUATOR_TESTS[i].name, 0);
                return UNITY_END();
            }
        }
        fprintf(stderr, "Unknown test: %s\n", argv[1]);
        return 1;
    }
    for (size_t i = 0; i < NUM_EVALUATOR_TESTS; i++) {
        UnityDefaultTestRun(EVALUATOR_TESTS[i].fn, EVALUATOR_TESTS[i].name, 0);
    }
    return UNITY_END();
}
