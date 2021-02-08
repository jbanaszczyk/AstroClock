
#include <Arduino.h>
#include <iostream>
#include <exception>

#include <check_exceptions.h>

void func(void) {
	throw std::runtime_error("exception thrown.");
}

void setup() {
	Serial.begin(115200);
	delay(2000);
	while (!Serial) {
	}
	Serial.println();
	Serial.println();

	Serial.println("_______go");

	check_try_catch();
}

void loop() {}
