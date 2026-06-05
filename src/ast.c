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

char *return_statement_to_string(ReturnStatement *rs) {
    StrBuf *sb = strbuf_new();
    strbuf_append(sb, "(return ");
    char *expr_str = expression_to_string(rs->value);
    strbuf_append(sb, expr_str);
    free(expr_str);
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

        switch (statement->type) {
        case EXPRESSION_STATEMENT:
            strbuf_append(sb, expression_statement_to_string(
                                  statement->expression_statement));
            break;
        case RETURN_STATEMENT:
            strbuf_append(
                sb, return_statement_to_string(statement->return_statement));
            break;
        }
    }

    return strbuf_done(sb);
}
