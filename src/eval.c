#include "eval.h"
#include <stdlib.h>
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
        return eval_expression_statement(s->expression_statement, env);
        break;
    default:
        return eval_error("Can't evaluate statement of type: %s",
                          statement_type_to_string(s->type));
        return NULL;
    }
}

Value *eval_expression_statement(ExpressionStatement *es, Environment *env) {
    return eval_expression(es->expression, env);
}

Value *eval_expression(Expression *e, Environment *env) {
    switch (e->type) {
    case EXPR_IF:
        return eval_if(e->if_expression, env);
    case EXPR_STRING:
        return eval_string(e->string_expression, env);
    case EXPR_NIL:
        // No need of nil-expression separate type
        return eval_nil(e, env);
    case EXPR_BOOL:
        return eval_bool(e->bool_expression, env);
    case EXPR_FLOAT:
        return eval_float(e->float_expression, env);
    case EXPR_INTEGER:
        return eval_integer(e->integer_expression, env);
        break;
    default:
        return eval_error("Can't evaluate expression of type: %s",
                          expression_type_to_string(e->type));
    }
}

Value *eval_integer(IntegerExpression *e, Environment *env) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_INTEGER;

    v->integer = e->value;

    return v;
}
Value *eval_float(FloatExpression *e, Environment *env) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_FLOAT;

    v->floating = e->value;

    return v;
}

Value *eval_bool(BoolExpression *e, Environment *env) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_BOOL;

    v->boolean = e->value;

    return v;
}

Value *eval_nil(Expression *e, Environment *env) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_NIL;

    return v;
}

Value *eval_string(StringExpression *e, Environment *env) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_STRING;

    v->string = e->value;

    return v;
}

Value *eval_if(IfExpression *e, Environment *env) {
    Value *result = eval_expression(e->cond, env);

    if (result->type != VAL_BOOL) {
        return eval_error(
            "Error: Expected expression to return bool, returned: %s",
            value_to_string(result));
    }

    if (result->boolean) {
        return eval_expression(e->consequence, env);
    } else if (e->alternative != NULL) {
        return eval_expression(e->alternative, env);
    }

    return make_nil();
}

Value *make_nil() {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_NIL;

    return v;
}
