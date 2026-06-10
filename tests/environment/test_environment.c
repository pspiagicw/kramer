#include "eval.h"
#include "object.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp() {}
void tearDown() {}

static Value *make_int(int n) {
    Value *v = malloc(sizeof(Value));
    v->type = VAL_INTEGER;
    v->integer = n;
    return v;
}

// A freshly created environment has no bindings.
void test_env_new_is_empty() {
    Environment *env = env_new(NULL);
    TEST_ASSERT_EQUAL_INT(0, env->count);
    TEST_ASSERT_NULL(env_get(env, "x"));
}

// Looking up a name that was never stored returns NULL.
void test_env_get_missing() {
    Environment *env = env_new(NULL);
    env_set(env, "x", make_int(1));
    TEST_ASSERT_NULL(env_get(env, "y"));
}

// A stored name can be retrieved with its value intact.
void test_env_set_and_get() {
    Environment *env = env_new(NULL);
    env_set(env, "x", make_int(42));

    Value *v = env_get(env, "x");
    TEST_ASSERT_NOT_NULL(v);
    TEST_ASSERT_EQUAL_INT(VAL_INTEGER, v->type);
    TEST_ASSERT_EQUAL_INT(42, v->integer);
}

// env_get returns the exact value pointer that was stored.
void test_env_get_returns_same_pointer() {
    Environment *env = env_new(NULL);
    Value *stored = make_int(7);
    env_set(env, "x", stored);
    TEST_ASSERT_EQUAL_PTR(stored, env_get(env, "x"));
}

// Multiple distinct names are each stored and retrieved independently.
void test_env_multiple_bindings() {
    Environment *env = env_new(NULL);
    env_set(env, "x", make_int(1));
    env_set(env, "y", make_int(2));
    env_set(env, "z", make_int(3));

    TEST_ASSERT_EQUAL_INT(1, env_get(env, "x")->integer);
    TEST_ASSERT_EQUAL_INT(2, env_get(env, "y")->integer);
    TEST_ASSERT_EQUAL_INT(3, env_get(env, "z")->integer);
}

// Re-assigning an existing name replaces its value without adding a slot.
void test_env_overwrite_existing() {
    Environment *env = env_new(NULL);
    env_set(env, "x", make_int(1));
    env_set(env, "x", make_int(99));

    TEST_ASSERT_EQUAL_INT(1, env->count);
    TEST_ASSERT_EQUAL_INT(99, env_get(env, "x")->integer);
}

// Storing more bindings than the initial capacity grows the environment.
void test_env_grows_past_initial_capacity() {
    Environment *env = env_new(NULL);
    char name[16];
    for (int i = 0; i < 50; i++) {
        snprintf(name, sizeof(name), "v%d", i);
        env_set(env, name, make_int(i));
    }

    TEST_ASSERT_EQUAL_INT(50, env->count);
    for (int i = 0; i < 50; i++) {
        snprintf(name, sizeof(name), "v%d", i);
        Value *v = env_get(env, name);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT(i, v->integer);
    }
}

typedef void (*TestFn)(void);
typedef struct {
    const char *name;
    TestFn fn;
} TestEntry;

static const TestEntry ENVIRONMENT_TESTS[] = {
    {"test_env_new_is_empty", test_env_new_is_empty},
    {"test_env_get_missing", test_env_get_missing},
    {"test_env_set_and_get", test_env_set_and_get},
    {"test_env_get_returns_same_pointer", test_env_get_returns_same_pointer},
    {"test_env_multiple_bindings", test_env_multiple_bindings},
    {"test_env_overwrite_existing", test_env_overwrite_existing},
    {"test_env_grows_past_initial_capacity",
     test_env_grows_past_initial_capacity},
};

#define NUM_ENVIRONMENT_TESTS                                                  \
    (sizeof(ENVIRONMENT_TESTS) / sizeof(ENVIRONMENT_TESTS[0]))

int main(int argc, char *argv[]) {
    UNITY_BEGIN();
    if (argc >= 2) {
        for (size_t i = 0; i < NUM_ENVIRONMENT_TESTS; i++) {
            if (strcmp(argv[1], ENVIRONMENT_TESTS[i].name) == 0) {
                UnityDefaultTestRun(ENVIRONMENT_TESTS[i].fn,
                                    ENVIRONMENT_TESTS[i].name, 0);
                return UNITY_END();
            }
        }
        fprintf(stderr, "Unknown test: %s\n", argv[1]);
        return 1;
    }
    for (size_t i = 0; i < NUM_ENVIRONMENT_TESTS; i++) {
        UnityDefaultTestRun(ENVIRONMENT_TESTS[i].fn, ENVIRONMENT_TESTS[i].name,
                            0);
    }
    return UNITY_END();
}
