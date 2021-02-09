
#include <Arduino.h>
#include <check_exceptions.h>
#include "globals.h"

void setup() {
	Serial.begin(115200);
	while (!Serial) {
	}
	Serial.println();

	delay(5000);

	Serial.println("[INIT] Setup");

	check_try_catch(false);
	configuration = getConfiguration(get_EEPROM_Rotate());

	Serial.println("[INIT] Good luck!");
}

void loop() {
}
