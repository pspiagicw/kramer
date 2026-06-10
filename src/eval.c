#include "eval.h"
#include <string.h>

Value *eval_ast(AST *ast, Environment *env) {
    Value *val = NULL;

    for (int i = 0; i < ast->numStatements; i++) {
        val = eval_statement(ast->statements[i], env);
    }

    return val;
}

Value *eval_statement(Statement *s, Environment *env) {
    switch (s->type) {
    case EXPRESSION_STATEMENT:
        eval_expression_statement(s->expression_statement, env);
        break;
    default:
        eval_error("Can't evaluate statement of type: %s",
                   statement_type_to_string(s->type));
        return NULL;
    }
    return NULL;
}

Value *eval_expression_statement(ExpressionStatement *es, Environment *env) {
    return eval_expression(es->expression, env);
}

Value *eval_expression(Expression *e, Environment *env) {
    switch (e->type) {
    default:
        eval_error("Can't evaluate expression of type: %s",
                   expression_type_to_string(e->type));
    }
    return NULL;
}
