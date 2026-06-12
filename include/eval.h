#pragma once

#include "ast.h"
#include "object.h"
#include <stdbool.h>

Environment *env_new(Environment *outer);
Value *env_get(Environment *env, const char *name);
void env_set(Environment *env, const char *name, Value *value);

Value *eval_ast(AST *ast, Environment *env);
Value *eval_statement(Statement *s, Environment *env);
Value *eval_expression(Expression *e, Environment *env);

Value *eval_expression_statement(ExpressionStatement *es, Environment *env);
Value *eval_expression(Expression *e, Environment *env);

Value *eval_integer(IntegerExpression *e, Environment *env);
Value *eval_float(FloatExpression *e, Environment *env);
Value *eval_bool(BoolExpression *e, Environment *env);
// No need of subtype of nli-expression;
Value *eval_nil(Expression *e, Environment *env);

Value *eval_string(StringExpression *e, Environment *env);

Value *eval_if(IfExpression *e, Environment *env);

Value *eval_call(CallExpression *e, Environment *env);
Value *eval_addition(Value **args, Environment *env, int numArgs);
Value *eval_subtract(Value **args, Environment *env, int numArgs);
Value *eval_multiply(Value **args, Environment *env, int numArgs);
Value *eval_divide(Value **args, Environment *env, int numArgs);

Value *eval_eq(Value **args, Environment *env, int numArgs);
Value *eval_neq(Value **args, Environment *env, int numArgs);

bool compareValues(Value *arg1, Value *arg2);

Value *make_nil();
