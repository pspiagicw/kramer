#include "ast.h"
#include "lexer.h"

typedef struct {
    char *Value;
} Error;

typedef struct {
    Lexer *lexer;
    AST *ast;
    Token *prev_token;
    Token *cur_token;
    Token *peek_token;
    Token *stashed_token;
    bool retreated;

    Error *errors;
    int numErrors;
} Parser;

Parser *newParser(Lexer *l);
void parser_parse(Parser *p);

void parser_advance(Parser *p);
void parser_retreat(Parser *p);
void parser_peek(Parser *p);

void parser_error(Parser *p, const char *fmt, ...);
Error *parser_errors(Parser *p);

void parse_statement(Parser *p);

Expression *parse_integer(Parser *p);
Expression *parse_float(Parser *p);
Expression *parse_string(Parser *p);
Expression *parse_bool(Parser *p);
Expression *parse_identifier(Parser *p);

Expression *parse_expression(Parser *p);

Expression *parse_call(Parser *p);

Expression *parse_if_expression(Parser *p);

Expression *parse_lparen_expression(Parser *p);

void parse_expression_statement(Parser *P);
void parse_return_statement(Parser *p);
void parse_let_statement(Parser *p);

AST *parser_ast(Parser *p);
Token *parser_expect(Parser *p, enum TokenType token_type);
