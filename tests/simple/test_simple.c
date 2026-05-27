#include "unity.h"

void setUp() {}
void tearDown() {}

void test_simple() {}

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_simple);
    return UNITY_END();
}
