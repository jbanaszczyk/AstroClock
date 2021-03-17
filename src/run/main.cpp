#include <Arduino.h>
#include <SimpleLogging.h>
#include "Globals.h"
#include "MVC.h"
#include "utilities.h"


#ifndef DEBUG_IOT_PORT
#ifdef DEBUG_ESP_PORT
#define DEBUG_IOT_PORT DEBUG_ESP_PORT
#else
#define DEBUG_IOT_PORT Serial
#endif
#endif

#ifndef DEBUG_IOT_LEVEL
#define DEBUG_IOT_LEVEL TRACE
#endif

void setupSerial(unsigned long baud = 74880) {
	Serial.begin(baud);
	while (!Serial) {}
	Serial.println();
}

void setup() {
	initFreeHeapSize = system_get_free_heap_size();
	setupSerial();

	Logging.addHandler(DEBUG_IOT_PORT, Logging.DEBUG_IOT_LEVEL);

	Serial.print(PSTR("==[ global setup ]==============\n"));

	initGlobals();

	Serial.print(PSTR("==[ model setup ]===============\n"));

	initEngine();

	Serial.print(PSTR("==[ setup done ]================\n"));
}

void loop() {
	static IRunner *runner = getRunner();
	runner->getScheduler()->execute();
}
