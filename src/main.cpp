#include <Arduino.h>
#include "Globals.h"
#include "MVC.h"
#include "utilities.h"
#include <TZ.h>

void setupSerial(unsigned long baud = 74880) {
	Serial.begin(baud);
	while (!Serial) {}
	Serial.println();
}

void setup() {
	initFreeHeapSize = system_get_free_heap_size();

	setupSerial();

	Serial.printf("==[ global setup ]==============\n");

	initGlobals();

	Serial.printf("==[ model setup ]===============\n");

	initEngine();

	Serial.printf("==[ setup done ]================\n");
}

// loop() is refactored to Runner.cpp
