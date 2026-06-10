#include "eval.h"
#include <stdlib.h>
#include <string.h>

Environment *env_new(Environment *outer) {
    Environment *e = malloc(sizeof(Environment));
    e->outer = outer;
    e->capacity = 10;
    e->count = 0;
    e->keys = malloc(sizeof(char *) * e->capacity);
    e->values = malloc(sizeof(Value *) * e->capacity);

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

void env_set(Environment *env, const char *name, Value *value) {

    int index = -1;
    for (int i = 0; i < env->count; i++) {
        if (strcmp(env->keys[i], name) == 0) {
            index = i;
            break;
        }
    }

    if (index >= 0) {
        // The key exists already, just update its value.
        env->values[index] = value;
    } else {
        // The key doens't exist. Own the key so callers can reuse/free theirs.
        env->keys[env->count] = strdup(name);
        env->values[env->count] = value;
        env->count++;
    }

    if (env->count == (env->capacity - 1)) {
        env->capacity *= 2;
        env->keys = realloc(env->keys, env->capacity * sizeof(char *));
        env->values = realloc(env->values, env->capacity * sizeof(char *));
    }
}
