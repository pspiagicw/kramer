#pragma once

#include "ast.h"
#include "token.h"

typedef struct Environment Environment;

enum ValueType {
    VAL_INTEGER,
    VAL_FLOAT,
    VAL_STRING,
    VAL_BOOL,
    VAL_NIL,
    VAL_FUNCTION,
    VAL_ERROR,
};

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
