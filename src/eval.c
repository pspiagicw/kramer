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
    case EXPR_CALL:
        return eval_call(e->call_expression, env);
        break;
    default:
        return eval_error("Can't evaluate expression of type: %s",
                          expression_type_to_string(e->type));
    }
}

Value *eval_call(CallExpression *e, Environment *env) {
    Token *caller = e->caller;
    Value **args = malloc(sizeof(Value *) * e->argumentNum);

    for (int i = 0; i < e->argumentNum; i++) {
        args[i] = eval_expression(e->argumentList[i], env);
    }

    switch (caller->Type) {
    case PLUS:
        return eval_addition(args, env, e->argumentNum);
    case MINUS:
        return eval_subtract(args, env, e->argumentNum);
    case MULTIPLY:
        return eval_multiply(args, env, e->argumentNum);
    case DIVIDE:
        return eval_divide(args, env, e->argumentNum);
    case EQ:
        return eval_eq(args, env, e->argumentNum);
    default:
        return eval_error("Call expression can't have %s as function to call.",
                          token_type_name(caller->Type));
    }
}

// Assumes arg1 and arg2 have same type!
bool compareValues(Value *arg1, Value *arg2) {
    switch (arg1->type) {
    case VAL_INTEGER:
        return arg1->integer == arg2->integer;
    case VAL_FLOAT:
        return arg1->floating == arg2->floating;
    case VAL_BOOL:
        return arg1->boolean == arg2->boolean;
    case VAL_STRING:
        return strcmp(arg1->string, arg2->string) == 0;
    default:
        return eval_error("Can't compare type: %s", value_to_string(arg1));
    }
    return false;
}

Value *eval_eq(Value **args, Environment *env, int numArgs) {
    bool result = true;

    if (numArgs < 2) {
        return eval_error("Can't compare less than 2 values!");
    }

    Value *first = args[0];
    for (int i = 1; i < numArgs; i++) {
        Value *val = args[i];
        if (val->type != first->type) {
            result = false;
        } else {
            result = result && compareValues(first, val);
        }
    }

    Value *res = malloc(sizeof(Value));
    res->type = VAL_BOOL;
    res->boolean = result;

    return res;
}

Value *eval_addition(Value **args, Environment *env, int numArgs) {
    float result = 0;
    bool is_float = false;

    for (int i = 0; i < numArgs; i++) {
        Value *val = args[i];
        if (val->type == VAL_INTEGER) {
            result += val->integer;
        } else if (val->type == VAL_FLOAT) {
            result += val->floating;
            is_float = true;
        } else {
            return eval_error("Can't perform addition on: %s",
                              value_to_string(val));
        }
    }

    Value *res = malloc(sizeof(Value));
    res->type = is_float ? VAL_FLOAT : VAL_INTEGER;
    if (is_float) {
        res->floating = result;
    } else {
        res->integer = result;
    }

    return res;
}

Value *eval_subtract(Value **args, Environment *env, int numArgs) {

    if (numArgs == 0) {
        return eval_error("Atleast 1 argument required for subtraction.");
    }

    if (numArgs == 1) {
        Value *val = args[0];
        if (val->type == VAL_INTEGER) {
            val->integer = -val->integer;
        } else if (val->type == VAL_FLOAT) {
            val->floating = -val->floating;
        } else {
            return eval_error("Can't perform subtraction on: %s",
                              value_to_string(val));
        }

        return val;
    }

    float result = 0;
    bool is_float = false;

    for (int i = 0; i < numArgs; i++) {
        Value *val = args[i];
        if (val->type == VAL_INTEGER) {
            // First case
            if (i == 0) {
                result = val->integer;
            } else {
                result = result - val->integer;
            }
        } else if (val->type == VAL_FLOAT) {
            if (i == 0) {
                result = val->floating;
            } else {
                result = result - val->floating;
            }
            is_float = true;
        } else {
            return eval_error("Can't perform subtraction on: %s",
                              value_to_string(val));
        }
    }

    Value *res = malloc(sizeof(Value));
    res->type = is_float ? VAL_FLOAT : VAL_INTEGER;
    if (is_float) {
        res->floating = result;
    } else {
        res->integer = result;
    }

    return res;
}

Value *eval_multiply(Value **args, Environment *env, int numArgs) {
    float result = 1;
    bool is_float = false;

    for (int i = 0; i < numArgs; i++) {
        Value *val = args[i];
        if (val->type == VAL_INTEGER) {
            result *= val->integer;
        } else if (val->type == VAL_FLOAT) {
            result *= val->floating;
            is_float = true;
        } else {
            return eval_error("Can't perform multiplication on: %s",
                              value_to_string(val));
        }
    }

    Value *res = malloc(sizeof(Value));
    res->type = is_float ? VAL_FLOAT : VAL_INTEGER;
    if (is_float) {
        res->floating = result;
    } else {
        res->integer = result;
    }

    return res;
}

Value *eval_divide(Value **args, Environment *env, int numArgs) {
    if (numArgs == 0) {
        return eval_error("Atleast 1 argument required for subtration.");
    }

    if (numArgs == 1) {
        Value *val = args[0];
        if (val->type == VAL_INTEGER) {
            if (val->integer == 0) {
                return eval_error("Can't divide by zero!");
            }
            val->floating = 1.0 / val->integer;
            val->type = VAL_FLOAT;
        } else if (val->type == VAL_FLOAT) {
            if (val->floating == 0) {
                return eval_error("Can't divide by zero!");
            }
            val->floating = 1.0 / val->floating;
        }

        return val;
    }

    float result = 0;

    for (int i = 0; i < numArgs; i++) {
        Value *val = args[i];
        if (val->type == VAL_INTEGER) {
            // First case
            if (i == 0) {
                result = val->integer;
            } else {
                result = result / val->integer;
            }
        } else if (val->type == VAL_FLOAT) {
            if (i == 0) {
                result = val->floating;
            } else {
                result /= val->floating;
            }
        } else {
            return eval_error("Can't perform division on: %s",
                              value_to_string(val));
        }
    }

    Value *res = malloc(sizeof(Value));
    res->floating = result;
    res->type = VAL_FLOAT;

    return res;
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
