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

	static auto initFreeHeap = ESP.getFreeHeap();
	//software interrupts
	WiFiManager.loop();

	//task A
	if (taskA.previous == 0 || (millis() - taskA.previous > taskA.rate)) {
		taskA.previous = millis();

		Serial.printf("Free heap memory: %d %d\n", ESP.getFreeHeap(), initFreeHeap);

		Serial.println("Before httpRequest");
		HTTPRequest httpRequest("https://www.google.com");
//		HTTPRequest httpRequest("http://www.google.com");
//		HTTPRequest httpRequest("https://api.ipgeolocation.io/timezone?apiKey=a8d4b9360d214b32bf057aaf1a6907ec");

		auto result = httpRequest.GET();
		Serial.printf("HTTP status: %d\n", result);

		while (httpRequest.busy()) {
			if (httpRequest.available()) {
				Serial.write(httpRequest.read());
			} else {
				delay(1);
			}
		}

		Serial.println("++++++++++++");
		httpRequest.end();
		Serial.println("++++++++++++");
	}
}



