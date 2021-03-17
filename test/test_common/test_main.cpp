
#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

void run_test_sunset();

int process() {
	UNITY_BEGIN();
	run_test_sunset();
	return UNITY_END();
}

#ifdef ARDUINO

#include <Arduino.h>

void setup() {
	// NOTE!!! Wait for >2 secs if board doesn't support software reset via Serial.DTR/RTS
	delay(2000);
	Serial.begin(115200);
	process();
}

void loop() {
}

#else

int main(int argc, char **argv) {
	process();
	return 0;
}

#endif
