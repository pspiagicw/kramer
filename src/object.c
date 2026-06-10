#include "object.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char *value_to_string(Value *v) { return ""; }

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
