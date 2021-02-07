#include <ArduinoFake.h>
#include <unity.h>

using namespace fakeit;

void test_setup(void) {
    When(Method(ArduinoFake(), pinMode)).Return();

    setup();

    Verify(Method(ArduinoFake(), pinMode).Using(LED_BUILTIN, OUTPUT)).Once();
}

void test_loop(void) {
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), delay)).AlwaysReturn();

    loop();

    Verify(Method(ArduinoFake(), digitalWrite).Using(LED_BUILTIN, HIGH)).Once();
    Verify(Method(ArduinoFake(), digitalWrite).Using(LED_BUILTIN, LOW)).Once();
    Verify(Method(ArduinoFake(), delay).Using(100)).Exactly(3_Times);
}

void setUp(void) {
    ArduinoFakeReset();
}

#define RUN_FAKE_TEST(test_name) try {                 \
        RUN_TEST(test_name);                           \
    } catch (const fakeit::VerificationException &e) { \
        TEST_FAIL_MESSAGE(e.what());                   \
    }

int main(int argc, char **argv) {
    UNITY_BEGIN();


    RUN_FAKE_TEST(test_setup);
    RUN_FAKE_TEST(test_loop);

//    RUN_TEST(test_setup);
//    RUN_TEST(test_loop);

    UNITY_END();
    return 0;
}
