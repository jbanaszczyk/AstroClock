
//const rst_info * resetInfo = system_get_rst_info();
//consolePort.print(F("system_get_rst_info() reset reason: "));
//consolePort.println(RST_REASONS[resetInfo->reason]);
//
//consolePort.print(F("system_get_free_heap_size(): "));
//consolePort.println(system_get_free_heap_size());
//
//const char * const RST_REASONS[] = {
//		"REASON_DEFAULT_RST",
//		"REASON_WDT_RST",
//		"REASON_EXCEPTION_RST",
//		"REASON_SOFT_WDT_RST",
//		"REASON_SOFT_RESTART",
//		"REASON_DEEP_SLEEP_AWAKE",
//		"REASON_EXT_SYS_RST"
//};

//void wifi_event_handler_cb(System_Event_t * event) {
//	ehConsolePort.print(EVENT_NAMES[event->event]);
//	ehConsolePort.print(" (");
//
//	switch (event->event) {
//		case EVENT_STAMODE_CONNECTED:
//			break;
//		case EVENT_STAMODE_DISCONNECTED:
//			break;
//		case EVENT_STAMODE_AUTHMODE_CHANGE:
//			break;
//		case EVENT_STAMODE_GOT_IP:
//			break;
//		case EVENT_SOFTAPMODE_STACONNECTED:
//		case EVENT_SOFTAPMODE_STADISCONNECTED: {
//			char mac[32] = {0};
//			snprintf(mac, 32, MACSTR ", aid: %d", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);
//
//			ehConsolePort.print(mac);
//		}
//			break;
//	}
//
//	ehConsolePort.println(")");
//}

// set_event_handler_cb_stream(Serial);
//wifi_set_event_handler_cb(wifi_event_handler_cb);










#include <Arduino.h>
#include <LittleFS.h>
#include <WiFiManager.h>
#include <webServer.h>
#include <OtaUpdateHelper.h>
#include <configManager.h>
#include <timeSync.h>
#include <TZ.h>
#include <SerialCommands.h>
#include <sntp.h>

#include "Globals.h"
#include "utilities.h"

void setupSerial(unsigned long baud = 74880) {
	Serial.begin(baud);
	while (!Serial) {}
	Serial.println();
}

void showNow() {
	if (timeSync.isSynced()) {
		time_t now;
		struct tm *info;
		time(&now);
		info = localtime(&now);
		char buffer[80];

		strftime(buffer, sizeof(buffer), "%Y.%m.%d %H.%M.%S", info);
		Serial.printf_P("Current local time: %s\n", buffer);
	} else {
		Serial.printf_P("Time is not ready\n");
	}
}


void setup() {
	setupSerial();

	Serial.printf("==[ global setup ]==============\n");

//	LittleFS.begin();
//	GUI.begin();
//	configManager.begin();
//	WiFiManager.begin(configManager.data.projectName);

//	timeSync.begin(TZ_Europe_Warsaw);
//	timeSync.waitForSyncResult(10000);

	commandProcessor = getCommandProcessor(new SerialCommands());
	commandProcessor->setStream(&Serial);
	wiFiMonitor = getWiFiMonitor();

	Serial.printf("==[ global setup done ]=========\n");
}

void loop() {
//	WiFiManager.loop();
//	otaUpdateHelper.loop();
//	configManager.loop();

	if (commandProcessor != nullptr) {
		commandProcessor->loop();
	}
//	delay(1000);
//	showNow();
}
