#pragma once

#include "ast.h"
#include <stdbool.h>

enum ValueType {
    VAL_INTEGER,
    VAL_FLOAT,
    VAL_STRING,
    VAL_BOOL,
    VAL_NIL,
    VAL_FUNCTION,
    VAL_ERROR,
};

typedef struct Environment Environment;

typedef struct {
    Token **params;
    int numParams;
    Statement *body;
    Environment *closure;
} FunctionValue;

typedef struct {
    enum ValueType type;
    union {
        int integer;
        float floating;
        char *string;
        bool boolean;
        FunctionValue *function;
        char *error_msg;
    };
} Value;

struct Environment {
    char **keys;
    Value **values;
    int count;
    int capacity;
    Environment *outer;
};

Environment *env_new(Environment *outer);
Value *env_get(Environment *env, const char *name);
void env_set(Environment *env, const char *name, Value *value);

Value *eval_ast(AST *ast, Environment *env);
Value *eval_statement(Statement *s, Environment *env);
Value *eval_expression(Expression *e, Environment *env);

char *value_to_string(Value *v);
