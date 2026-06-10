#pragma once

#include "token.h"
#include <stdbool.h>

enum StatementType {
    RETURN_STATEMENT,
    EXPRESSION_STATEMENT,
    LET_STATEMENT,
    FUNCTION_STATEMENT
};
enum ExpressionType {
    EXPR_INTEGER,
    EXPR_FLOAT,
    EXPR_STRING,
    EXPR_BOOL,
    EXPR_IDENT,
    EXPR_CALL,
    EXPR_IF,
    EXPR_LAMBDA,
    EXPR_NIL,
};

enum StringType { SINGLE, DOUBLE, MULTILINE };

typedef struct Expression Expression;
typedef struct Statement Statement;

typedef struct {
    Token *caller;
    struct Expression **argumentList;
    int argumentNum;
} CallExpression;

typedef struct {
    char *value;
} IdentifierExpression;

typedef struct {
    bool value;
} BoolExpression;

typedef struct {
    int value;
} IntegerExpression;

typedef struct {
    float value;
} FloatExpression;

typedef struct {
    char *value;
    enum StringType Type;
} StringExpression;

typedef struct {
    struct Expression *cond;
    struct Expression *consequence;
    struct Expression *alternative;
} IfExpression;

typedef struct {
    Token **args;
    int numArgs;

    struct Statement *statement;
} LambdaExpression;

struct Expression {
    enum ExpressionType type;
    union {
        IntegerExpression *integer_expression;
        FloatExpression *float_expression;
        StringExpression *string_expression;
        BoolExpression *bool_expression;
        IdentifierExpression *identifier_expression;
        CallExpression *call_expression;
        IfExpression *if_expression;
        LambdaExpression *lambda_expression;
    };
};

typedef struct {
    Expression *value;
} ReturnStatement;

typedef struct {
    Expression *expression;
} ExpressionStatement;

typedef struct {
    Token *name;
    Expression *Value;
} LetStatement;

typedef struct {
    Token *name;
    Token **args;
    int numArgs;

    struct Statement *statement;
} FunctionStatement;

void add_arg_to_fn(FunctionStatement *fn, Token *arg);
void add_arg_to_lambda(LambdaExpression *l, Token *arg);

struct Statement {
    enum StatementType type;
    union {
        ReturnStatement *return_statement;
        ExpressionStatement *expression_statement;
        LetStatement *let_statement;
        FunctionStatement *function_statement;
    };
};

typedef struct {
    Statement **statements;
    int numStatements;
} AST;

void add_statement(AST *ast, Statement *statement);

void add_argument_to_call(CallExpression *c, Expression *argument);

char *ast_to_string(AST *ast);

char *return_statement_to_string(ReturnStatement *return_statement);
char *let_statement_to_string(LetStatement *let_statement);
char *expression_statement_to_string(ExpressionStatement *expression_statement);

char *expression_to_string(Expression *expression);

char *integer_to_string(IntegerExpression *expr);
char *string_to_string(StringExpression *expr);
char *float_to_string(FloatExpression *expr);
char *bool_to_string(BoolExpression *expr);
char *identifier_to_string(IdentifierExpression *expr);

char *call_to_string(CallExpression *expr);
char *if_to_string(IfExpression *expr);
char *fn_to_string(FunctionStatement *fn);
char *lambda_to_string(LambdaExpression *expr);
char *statement_to_string(Statement *statement);

const char *statement_type_to_string(enum StatementType type);
const char *expression_type_to_string(enum ExpressionType type);
