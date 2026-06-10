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
