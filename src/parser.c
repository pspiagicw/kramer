#include "parser.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

Parser *newParser(Lexer *l) {
    Parser *p = malloc(sizeof(Parser));

    p->lexer = l;
    p->ast = malloc(sizeof(AST));
    p->ast->statements = NULL;
    p->ast->numStatements = 0;
    p->errors = NULL;
    p->numErrors = 0;
    p->prev_token = NULL;
    p->stashed_token = NULL;
    p->retreated = false;
    p->peek_token = lexer_next(p->lexer);

    parser_advance(p);

    return p;
}

AST *parser_ast(Parser *p) { return p->ast; }

void parser_advance(Parser *p) {
    p->prev_token = p->cur_token;
    p->cur_token = p->peek_token;
    if (p->retreated) {
        p->peek_token = p->stashed_token;
        p->stashed_token = NULL;
        p->retreated = false;
    } else {
        p->peek_token = lexer_next(p->lexer);
    }
}

void parser_retreat(Parser *p) {
    p->stashed_token = p->peek_token;
    p->peek_token = p->cur_token;
    p->cur_token = p->prev_token;
    p->prev_token = NULL;
    p->retreated = true;
}

void parser_parse(Parser *p) {
    while (p->cur_token->Type != TOKEN_EOF) {
        switch (p->cur_token->Type) {
        case LPAREN:
            parse_statement(p);
            break;
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

void parse_return_statement(Parser *p) {
    Statement *s = malloc(sizeof(Statement));
    s->type = RETURN_STATEMENT;
    s->return_statement = malloc(sizeof(ReturnStatement));

    // Move over the return token.
    parser_advance(p);

    s->return_statement->value = parse_expression(p);

    parser_expect(p, RPAREN);

    add_statement(p->ast, s);
}

void parser_expect(Parser *p, enum TokenType token_type) {
    if (p->cur_token->Type != token_type) {
        parser_error(p, "Expected %s, got %s", token_type_name(token_type),
                     token_type_name(p->cur_token->Type));
    }
    parser_advance(p);
}

Expression *parse_integer(Parser *p) {
    IntegerExpression *i = malloc(sizeof(IntegerExpression));
    i->value = atoi(p->cur_token->Value);

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_INTEGER;

    e->integer_expression = i;

    parser_advance(p);

    return e;
}

Expression *parse_float(Parser *p) {
    FloatExpression *f = malloc(sizeof(FloatExpression));
    f->value = atof(p->cur_token->Value);

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_FLOAT;

    e->float_expression = f;

    parser_advance(p);

    return e;
}

Expression *parse_bool(Parser *p) {
    BoolExpression *b = malloc(sizeof(BoolExpression));

    switch (p->cur_token->Type) {
    case TRUE:
        b->value = true;
        break;
    case FALSE:
        b->value = false;
        break;
    default:
        parser_error(p, "Invalid bool type!, got %s",
                     token_type_name(p->cur_token->Type));
    }

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_BOOL;

    e->bool_expression = b;
    parser_advance(p);

    return e;
}

Expression *parse_identifier(Parser *p) {
    IdentifierExpression *i = malloc(sizeof(IdentifierExpression));
    i->value = p->cur_token->Value;

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_IDENT;

    e->identifier_expression = i;
    parser_advance(p);

    return e;
}

Expression *parse_string(Parser *p) {
    StringExpression *s = malloc(sizeof(StringExpression));
    s->value = p->cur_token->Value;

    switch (p->cur_token->Type) {
    case STRING_SINGLE:
        s->Type = SINGLE;
        break;
    case STRING_DOUBLE:
        s->Type = DOUBLE;
        break;
    case STRING_MULTILINE:
        s->Type = MULTILINE;
        break;
    default:
        parser_error(p, "Invalid string type! got %s",
                     token_type_name(p->cur_token->Type));
    }

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_STRING;

    e->string_expression = s;

    parser_advance(p);
    return e;
}

Expression *parse_call(Parser *p) {
    // Move over (
    parser_advance(p);

    CallExpression *c = malloc(sizeof(CallExpression));
    c->caller = p->cur_token;

    // Move over the call ident.
    parser_advance(p);

    c->argumentNum = 0;
    c->argumentList = malloc(sizeof(Expression **));

    while (p->cur_token->Type != RPAREN) {
        add_argument_to_call(c, parse_expression(p));
    }

    // Move over the )
    parser_advance(p);

    Expression *e = malloc(sizeof(Expression));
    e->type = EXPR_CALL;

    e->call_expression = c;

    return e;
}

void parse_statement(Parser *p) {
    parser_advance(p);

    switch (p->cur_token->Type) {
    case RETURN:
        parse_return_statement(p);
        break;
    case IDENTIFIER:
    case MINUS:
    case MULTIPLY:
    case DIVIDE:
    case MODULO:
    case EQ:
    case NEQ:
    case LT:
    case GT:
    case LTE:
    case GTE:
    case AND:
    case OR:
    case NOT:
    case PLUS:
    case CONCAT:
        // It's a call expression statement now!
        parser_retreat(p);
        // Now expression statement will take (..) as expression.
        parse_expression_statement(p);
        break;
    default:
        parser_error(p, "No statement can be parsed with %s",
                     p->cur_token->Value);
        parser_advance(p);
    }
}

Expression *parse_expression(Parser *p) {
    switch (p->cur_token->Type) {
    case INTEGER:
        return parse_integer(p);
    case FLOAT:
        return parse_float(p);
    case STRING_DOUBLE:
    case STRING_MULTILINE:
    case STRING_SINGLE:
        return parse_string(p);
    case TRUE:
    case FALSE:
        return parse_bool(p);
    case IDENTIFIER:
        return parse_identifier(p);
    case LPAREN:
        return parse_call(p);
    default:
        parser_error(p, "No expression can be parsed with %s",
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
