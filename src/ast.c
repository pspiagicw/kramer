#include "ast.h"
#include "strbuf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void add_statement(AST *ast, Statement *statement) {
    ast->statements =
        realloc(ast->statements, (ast->numStatements + 1) * sizeof(Statement));
    ast->statements[ast->numStatements++] = *statement;
}

void add_argument_to_call(CallExpression *c, Expression *argument) {
    c->argumentList =
        realloc(c->argumentList, (c->argumentNum + 1) * sizeof(Expression *));
    c->argumentList[c->argumentNum++] = argument;
}

void add_arg_to_fn(FunctionStatement *f, Token *arg) {
    f->args = realloc(f->args, (f->numArgs + 1) * sizeof(Token *));
    f->args[f->numArgs++] = arg;
}

void add_arg_to_lambda(LambdaExpression *l, Token *arg) {
    l->args = realloc(l->args, (l->numArgs + 1) * sizeof(Token *));
    l->args[l->numArgs++] = arg;
}

char *expression_to_string(Expression *expr) {
    switch (expr->type) {
    case EXPR_INTEGER:
        return integer_to_string(expr->integer_expression);
    case EXPR_FLOAT:
        return float_to_string(expr->float_expression);
    case EXPR_STRING:
        return string_to_string(expr->string_expression);
    case EXPR_BOOL:
        return bool_to_string(expr->bool_expression);
    case EXPR_IDENT:
        return identifier_to_string(expr->identifier_expression);
    case EXPR_CALL:
        return call_to_string(expr->call_expression);
    case EXPR_IF:
        return if_to_string(expr->if_expression);
    case EXPR_LAMBDA:
        return lambda_to_string(expr->lambda_expression);
    default:
        return "";
    }
}

char *integer_to_string(IntegerExpression *expr) {
    StrBuf *sb = strbuf_new();
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", expr->value);
    strbuf_append(sb, buf);
    return strbuf_done(sb);
}

char *float_to_string(FloatExpression *expr) {
    StrBuf *sb = strbuf_new();
    char buf[32];
    snprintf(buf, sizeof(buf), "%.1f", expr->value);
    strbuf_append(sb, buf);
    return strbuf_done(sb);
}

char *string_to_string(StringExpression *expr) {
    StrBuf *sb = strbuf_new();
    int length = strlen(expr->value);
    char buf[length + 10];

    switch (expr->Type) {
    case SINGLE:
        snprintf(buf, sizeof(buf), "'%s'", expr->value);
        break;
    case DOUBLE:
        snprintf(buf, sizeof(buf), "\"%s\"", expr->value);
        break;
    case MULTILINE:
        snprintf(buf, sizeof(buf), "[[%s]]", expr->value);
        break;
    }

    strbuf_append(sb, buf);
    return strbuf_done(sb);
}

char *bool_to_string(BoolExpression *expr) {
    StrBuf *sb = strbuf_new();
    char buf[32];
    snprintf(buf, sizeof(buf), "%s", expr->value ? "true" : "false");
    strbuf_append(sb, buf);
    return strbuf_done(sb);
}

char *identifier_to_string(IdentifierExpression *expr) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, expr->value);
    return strbuf_done(sb);
}

char *call_to_string(CallExpression *expr) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(");
    strbuf_append(sb, expr->caller->Value);
    for (int i = 0; i < expr->argumentNum; i++) {
        strbuf_append(sb, " ");
        char *arg = expression_to_string(expr->argumentList[i]);
        strbuf_append(sb, arg);
        free(arg);
    }
    strbuf_append(sb, ")");
    return strbuf_done(sb);
}

char *if_to_string(IfExpression *expr) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(if ");
    char *cond = expression_to_string(expr->cond);
    strbuf_append(sb, cond);
    free(cond);
    strbuf_append(sb, " ");
    char *cons = expression_to_string(expr->consequence);
    strbuf_append(sb, cons);
    free(cons);
    if (expr->alternative != NULL) {
        strbuf_append(sb, " ");
        char *alt = expression_to_string(expr->alternative);
        strbuf_append(sb, alt);
        free(alt);
    }
    strbuf_append(sb, ")");
    return strbuf_done(sb);
}

char *fn_to_string(FunctionStatement *fn);

char *statement_to_string(Statement *s);

char *lambda_to_string(LambdaExpression *expr) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(lambda (");
    for (int i = 0; i < expr->numArgs; i++) {
        if (i != 0) strbuf_append(sb, " ");
        strbuf_append(sb, expr->args[i]->Value);
    }
    strbuf_append(sb, ")");
    if (expr->statement != NULL) {
        strbuf_append(sb, " ");
        char *body = statement_to_string(expr->statement);
        strbuf_append(sb, body);
        free(body);
    }
    strbuf_append(sb, ")");
    return strbuf_done(sb);
}

char *statement_to_string(Statement *s) {
    switch (s->type) {
    case EXPRESSION_STATEMENT:
        return expression_statement_to_string(s->expression_statement);
    case RETURN_STATEMENT:
        return return_statement_to_string(s->return_statement);
    case LET_STATEMENT:
        return let_statement_to_string(s->let_statement);
    case FUNCTION_STATEMENT:
        return fn_to_string(s->function_statement);
    }
    return "";
}

char *fn_to_string(FunctionStatement *fn) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(fn ");
    strbuf_append(sb, fn->name->Value);
    strbuf_append(sb, " (");
    for (int i = 0; i < fn->numArgs; i++) {
        if (i != 0)
            strbuf_append(sb, " ");
        strbuf_append(sb, fn->args[i]->Value);
    }
    strbuf_append(sb, ")");
    if (fn->statement != NULL) {
        strbuf_append(sb, " ");
        char *body = statement_to_string(fn->statement);
        strbuf_append(sb, body);
        free(body);
    }
    strbuf_append(sb, ")");
    return strbuf_done(sb);
}

char *return_statement_to_string(ReturnStatement *rs) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(return ");
    char *expr_str = expression_to_string(rs->value);
    strbuf_append(sb, expr_str);
    free(expr_str);
    strbuf_append(sb, ")");
    return strbuf_done(sb);
}

char *let_statement_to_string(LetStatement *let) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(let ");
    char *name_str = let->name->Value;
    strbuf_append(sb, name_str);
    strbuf_append(sb, " ");
    strbuf_append(sb, expression_to_string(let->Value));
    strbuf_append(sb, ")");

    return strbuf_done(sb);
}

char *expression_statement_to_string(ExpressionStatement *es) {
    return expression_to_string(es->expression);
}

char *ast_to_string(AST *ast) {

    StrBuf *sb = strbuf_new();
    for (int i = 0; i < ast->numStatements; i++) {
        Statement *statement = &ast->statements[i];

        if (i != 0 && i < ast->numStatements) {
            strbuf_append(sb, " ");
        }

        switch (statement->type) {
        case EXPRESSION_STATEMENT:
            strbuf_append(sb, expression_statement_to_string(
                                  statement->expression_statement));
            break;
        case RETURN_STATEMENT:
            strbuf_append(
                sb, return_statement_to_string(statement->return_statement));
            break;
        case LET_STATEMENT:
            strbuf_append(sb,
                          let_statement_to_string(statement->let_statement));
            break;
        case FUNCTION_STATEMENT:
            strbuf_append(sb, fn_to_string(statement->function_statement));
            break;
        }
    }

    return strbuf_done(sb);
}
