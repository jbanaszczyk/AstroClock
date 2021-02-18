#include <Arduino.h>
#include "LittleFS.h"

#include "WiFiManager.h"
#include "webServer.h"
#include "updater.h"
#include "fetch.h"
#include "configManager.h"
#include "timeSync.h"


struct task {
	unsigned long rate;
	unsigned long previous;
};

task taskA = {.rate = 10000, .previous = 0};

void setup() {
	Serial.begin(115200);

	LittleFS.begin();
	GUI.begin();
	configManager.begin();
	WiFiManager.begin(configManager.data.projectName);
	timeSync.begin();
}

void loop() {
	//software interrupts
	WiFiManager.loop();

	//task A
	if (taskA.previous == 0 || (millis() - taskA.previous > taskA.rate)) {
		taskA.previous = millis();

		Serial.println("Before fetch");
//		auto result = fetch.GET("https://api.ipgeolocation.io/timezone?apiKey=a8d4b9360d214b32bf057aaf1a6907ec");
		auto result = fetch.GET("https://www.google.com");
		Serial.printf("HTTP status: %d\n", result);

		while (fetch.busy()) {
			if (fetch.available()) {
				Serial.write(fetch.read());
			} else {
				delay(1);
			}
		}

		fetch.clean();
	}
}



