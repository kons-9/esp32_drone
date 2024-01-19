#include <Arduino.h>
#include <unity.h>

#ifdef NATIVE
using namespace fakeit;
#endif

void setUp(void) {
#ifdef NATIVE
    ArduinoFakeReset();
#endif
}

void tearDown(void) {
    // clean stuff up here
}

void test_sample(void) {
    TEST_ASSERT_EQUAL(3, 3);
}

void run(void) {
    RUN_TEST(test_sample);
}


#ifdef NATIVE
int main(int argc, char **argv) {
    UNITY_BEGIN();

    run();

    return UNITY_END();
}
#else
void setup() {
    delay(2000);
    UNITY_BEGIN();

    run();

    UNITY_END();
}
void loop() {
}
#endif