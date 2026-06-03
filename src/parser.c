#include "parser.h"
#include "strbuf.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Parser *newParser(Lexer *l) {
    Parser *p = malloc(sizeof(Parser));

    p->lexer = l;
    p->ast = malloc(sizeof(AST));
    p->ast->statements = malloc(sizeof(Statement *) * 10);
    p->peek_token = lexer_next(p->lexer);

    parser_advance(p);

    return p;
}

AST *parser_ast(Parser *p) { return p->ast; }

void parser_advance(Parser *p) {
    p->cur_token = p->peek_token;
    p->peek_token = lexer_next(p->lexer);
}

void parser_parse(Parser *p) {
    while (p->cur_token->Type != TOKEN_EOF) {
        switch (p->cur_token->Type) {
        case LPAREN:
            parse_statement(p);
        default:
            parse_expression_statement(p);
        }
    }
}

void parse_expression_statement(Parser *p) {
    Statement *s = malloc(sizeof(Statement));
    s->type = EXPRESSION_STATEMENT;
    s->expression_statement = malloc(sizeof(ExpressionStatement));
    s->expression_statement->expression = parse_expression(p);

    add_statement(p->ast, s);
}

Expression *parse_integer(Parser *p) {
    IntegerExpression *i = malloc(sizeof(IntegerExpression));
    i->value = atoi(p->cur_token->Value);

    Expression *e = malloc(sizeof(Expression));

    e->integer_expression = i;

    parser_advance(p);

    return e;
}

void parse_statement(Parser *p) { parser_advance(p); }

Expression *parse_expression(Parser *p) {
    switch (p->cur_token->Type) {
    case INTEGER:
        return parse_integer(p);

    default:
        parser_error(p, "No statement can be parsed with %s",
                     p->cur_token->Value);
        parser_advance(p);
    }
    return NULL;
}

void parser_error(Parser *p, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char *msg = malloc(len + 1);

    va_start(args, fmt);
    vsnprintf(msg, len + 1, fmt, args);
    va_end(args);

    p->errors = realloc(p->errors, (p->numErrors + 1) * sizeof(Error));
    p->errors[p->numErrors++].Value = msg;
}

Error *parser_errors(Parser *p) { return p->errors; }
