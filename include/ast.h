#pragma once

#include "token.h"
#include <stdbool.h>

enum StatementType { RETURN_STATEMENT, EXPRESSION_STATEMENT, LET_STATEMENT };
enum ExpressionType {
    EXPR_INTEGER,
    EXPR_FLOAT,
    EXPR_STRING,
    EXPR_BOOL,
    EXPR_IDENT,
    EXPR_CALL,
    EXPR_IF,
};

enum StringType { SINGLE, DOUBLE, MULTILINE };

struct Expression;

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

typedef struct Expression {
    enum ExpressionType type;
    union {
        IntegerExpression *integer_expression;
        FloatExpression *float_expression;
        StringExpression *string_expression;
        BoolExpression *bool_expression;
        IdentifierExpression *identifier_expression;
        CallExpression *call_expression;
        IfExpression *if_expression;
    };
} Expression;

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
    enum StatementType type;
    union {
        ReturnStatement *return_statement;
        ExpressionStatement *expression_statement;
        LetStatement *let_statement;
    };
} Statement;

typedef struct {
    Statement *statements;
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
