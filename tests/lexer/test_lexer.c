#include "lexer.h"
#include "unity.h"

void setUp() {}
void tearDown() {}

void testTokens(char *input, Token *expectedTokens) {
    Lexer *l = newLexer(input);

    int counter = 0;

    while (l->isEOF == false) {
        Token *actualToken = lexer_next(l);
        Token expectedToken = expectedTokens[counter];

        TEST_ASSERT_EQUAL_STRING(token_type_name(expectedToken.Type), token_type_name(actualToken->Type));
        TEST_ASSERT_EQUAL_STRING(expectedToken.Value, actualToken->Value);

        counter += 1;
    }
}

void test_single_symbol() {
    char *input = "   +";

    Token expectedTokens[] = {
        {PLUS, "+"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_symbols() {
    char *input = "- + * / ! % ^ , .";

    Token expectedTokens[] = {
        {MINUS, "-"}, {PLUS, "+"},     {MULTIPLY, "*"}, {DIVIDE, "/"},
        {BANG, "!"},  {MODULO, "%"},   {EXPONENT, "^"}, {COMMA, ","},
        {DOT, "."},   {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_comparison_operators() {
    char *input = "== != <= >=";

    Token expectedTokens[] = {
        {EQ, "=="}, {NEQ, "!="}, {LTE, "<="}, {GTE, ">="}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_single_char_comparison_assignment() {
    char *input = "< > =";

    Token expectedTokens[] = {
        {LT, "<"},
        {GT, ">"},
        {ASSIGN, "="},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_delimiters() {
    char *input = "( ) { } [ ]";

    Token expectedTokens[] = {
        {LPAREN, "("},  {RPAREN, ")"},  {LBRACE, "{"},   {RBRACE, "}"},
        {LSQUARE, "["}, {RSQUARE, "]"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_dots() {
    char *input = ".. ...";

    Token expectedTokens[] = {
        {CONCAT, ".."},
        {ELLIPSIS, "..."},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_identifiers() {
    char *input = "foo bar _baz";

    Token expectedTokens[] = {
        {IDENTIFIER, "foo"},
        {IDENTIFIER, "bar"},
        {IDENTIFIER, "_baz"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_numbers() {
    char *input = "123 3.14";

    Token expectedTokens[] = {
        {INTEGER, "123"},
        {FLOAT, "3.14"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_keywords() {
    char *input = "if else return fn end let not and or then";

    Token expectedTokens[] = {
        {IF, "if"},   {ELSE, "else"}, {RETURN, "return"}, {FN, "fn"},
        {END, "end"}, {LET, "let"},   {NOT, "not"},       {AND, "and"},
        {OR, "or"},   {THEN, "then"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_literals() {
    char *input = "nil true false";

    Token expectedTokens[] = {
        {NIL, "nil"},
        {TRUE, "true"},
        {FALSE, "false"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_strings() {
    char *input = "\"hello\" 'world' [[long string]]";

    Token expectedTokens[] = {
        {STRING_DOUBLE, "hello"},
        {STRING_SINGLE, "world"},
        {STRING_MULTILINE, "long string"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_single_line_comment_only() {
    char *input = "-- this is a comment";

    Token expectedTokens[] = {
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_multiline_comment_only() {
    char *input = "--[[ this is a \n multiline comment ]]";

    Token expectedTokens[] = {
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_multiline_comment_between_code() {
    char *input = "(let y 10)\n"
                  "--[[\n"
                  "  this comment should be ignored\n"
                  "]]\n"
                  "(+ y 1)";

    Token expectedTokens[] = {
        {LPAREN, "("},  {LET, "let"},  {IDENTIFIER, "y"}, {INTEGER, "10"},
        {RPAREN, ")"},  {LPAREN, "("}, {PLUS, "+"},       {IDENTIFIER, "y"},
        {INTEGER, "1"}, {RPAREN, ")"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_array() {
    char *input = "[1,2,3]";

    Token expectedTokens[] = {
        {LSQUARE, "["}, {INTEGER, "1"}, {COMMA, ","},   {INTEGER, "2"},
        {COMMA, ","},   {INTEGER, "3"}, {RSQUARE, "]"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}
void test_types() {
    char *input = "int float string bool void";

    Token expectedTokens[] = {
        {TYPE, "int"},  {TYPE, "float"}, {TYPE, "string"},
        {TYPE, "bool"}, {TYPE, "void"},  {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_negative_numbers() {
    char *input = "-42 -3.14";

    Token expectedTokens[] = {
        {INTEGER, "-42"},
        {FLOAT, "-3.14"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_nested_sexpr() {
    char *input = "(+ 1 (- 3 2))";

    Token expectedTokens[] = {
        {LPAREN, "("}, {PLUS, "+"},     {INTEGER, "1"}, {LPAREN, "("},
        {MINUS, "-"},  {INTEGER, "3"},  {INTEGER, "2"}, {RPAREN, ")"},
        {RPAREN, ")"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_empty_list() {
    char *input = "()";

    Token expectedTokens[] = {
        {LPAREN, "("},
        {RPAREN, ")"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_hyphenated_identifier() {
    char *input = "my-func is-null";

    Token expectedTokens[] = {
        {IDENTIFIER, "my-func"},
        {IDENTIFIER, "is-null"},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_quote_shorthand() {
    char *input = "'foo '(1 2 3)";

    Token expectedTokens[] = {
        {QUOTE, "'"},   {IDENTIFIER, "foo"}, {QUOTE, "'"},
        {LPAREN, "("},  {INTEGER, "1"},      {INTEGER, "2"},
        {INTEGER, "3"}, {RPAREN, ")"},       {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_consecutive_expressions() {
    char *input = "(+ 1 2)(- 3 4)";

    Token expectedTokens[] = {
        {LPAREN, "("},  {PLUS, "+"},   {INTEGER, "1"},  {INTEGER, "2"},
        {RPAREN, ")"},  {LPAREN, "("}, {MINUS, "-"},    {INTEGER, "3"},
        {INTEGER, "4"}, {RPAREN, ")"}, {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

void test_empty_string() {
    char *input = "\"\"";

    Token expectedTokens[] = {
        {STRING_DOUBLE, ""},
        {TOKEN_EOF, ""},
    };

    testTokens(input, expectedTokens);
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_single_symbol);
    RUN_TEST(test_symbols);
    RUN_TEST(test_comparison_operators);
    RUN_TEST(test_single_char_comparison_assignment);
    RUN_TEST(test_delimiters);
    RUN_TEST(test_dots);
    RUN_TEST(test_identifiers);
    RUN_TEST(test_numbers);
    RUN_TEST(test_keywords);
    RUN_TEST(test_literals);
    RUN_TEST(test_strings);
    RUN_TEST(test_single_line_comment_only);
    RUN_TEST(test_multiline_comment_only);
    RUN_TEST(test_multiline_comment_between_code);
    RUN_TEST(test_array);
    RUN_TEST(test_types);
    RUN_TEST(test_negative_numbers);
    RUN_TEST(test_nested_sexpr);
    RUN_TEST(test_empty_list);
    RUN_TEST(test_hyphenated_identifier);
    RUN_TEST(test_quote_shorthand);
    RUN_TEST(test_consecutive_expressions);
    RUN_TEST(test_empty_string);
    return UNITY_END();
}
