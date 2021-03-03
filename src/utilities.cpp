#include "utilities.h"

#include <Arduino.h>

#include "utilities.h"

void blink(int n) {
	pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
	for (auto counter = n; counter > 0; --counter) {
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(2000);
	}
}
