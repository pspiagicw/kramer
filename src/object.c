#include "object.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *value_to_string(Value *v) {
    char buf[64];

    switch (v->type) {
    case VAL_INTEGER:
        snprintf(buf, sizeof(buf), "%d", v->integer);
        return strdup(buf);
    case VAL_FLOAT:
        snprintf(buf, sizeof(buf), "%g", v->floating);
        return strdup(buf);
    case VAL_BOOL:
        return strdup(v->boolean ? "true" : "false");
    case VAL_STRING:
        return strdup(v->string);
    case VAL_NIL:
        return strdup("nil");
    case VAL_FUNCTION:
        return strdup("<function>");
    case VAL_ERROR:
        return strdup(v->error_msg);
    }

    return strdup("<unknown>");
}

Value *eval_error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char *msg = malloc(len + 1);
    va_start(args, fmt);
    vsnprintf(msg, len + 1, fmt, args);
    va_end(args);

    fprintf(stderr, "Error: %s", msg);
    exit(1);
}
