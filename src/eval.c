#include "eval.h"
#include <stdlib.h>
#include <string.h>

Environment *env_new(Environment *outer) {
    Environment *e = malloc(sizeof(Environment));
    e->outer = outer;
    e->keys = malloc(sizeof(char *) * 10);
    e->values = malloc(sizeof(Value *) * 10);
    e->count = 0;
    e->capacity = 10;

    return e;
}

Value *env_get(Environment *env, const char *name) {
    int index = -1;
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->keys[i], name) == 0) {
            index = i;
            break;
        }
    }

    if (index < 0) {
        return NULL;
    }

    return env->values[index];
}
