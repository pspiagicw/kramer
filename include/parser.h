#include "ast.h"
#include "lexer.h"

typedef struct {
    char *Value;
} Error;

typedef struct {
    Lexer *lexer;
    AST *ast;
    Token *cur_token;
    Token *peek_token;

    Error *errors;
    int numErrors;
} Parser;

Parser *newParser(Lexer *l);
void parser_parse(Parser *p);

void parser_advance(Parser *p);
void parser_peek(Parser *p);

void parser_error(Parser *p, const char *fmt, ...);
Error *parser_errors(Parser *p);

void parse_statement(Parser *p);
Expression *parse_integer(Parser *p);

Expression *parse_expression(Parser *p);

void parse_expression_statement(Parser *P);

AST *parser_ast(Parser *p);
