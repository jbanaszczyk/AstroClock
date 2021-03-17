
#include <unity.h>

#include "test_main.h"

//void setUp(void) {
//    TEST_MESSAGE("Within setUp");
//}
//
//void tearDown(void) {
//    TEST_MESSAGE("Within tearDown");
//}

int test_runner() {
    UNITY_BEGIN();
    test_runner_calculator();
    return UNITY_END();
}

int main(int argc, char **argv) {
    return test_runner();
}
