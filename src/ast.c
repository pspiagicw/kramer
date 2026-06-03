#include "ast.h"
#include "strbuf.h"
#include <stdio.h>
#include <stdlib.h>

void add_statement(AST *ast, Statement *statement) {
    ast->statements =
        realloc(ast->statements, (ast->numStatements + 1) * sizeof(Statement));
    ast->statements[ast->numStatements++] = *statement;
}

char *expression_to_string(Expression *expr) {
    StrBuf *sb = strbuf_new();
    switch (expr->type) {
    case EXPR_INTEGER: {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", expr->integer_expression->value);
        strbuf_append(sb, buf);
        break;
    }
    }
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
