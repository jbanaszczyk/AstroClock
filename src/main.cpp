
//const rst_info * resetInfo = system_get_rst_info();
//consolePort.print(F("system_get_rst_info() reset reason: "));
//consolePort.println(RST_REASONS[resetInfo->reason]);
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


#include <Arduino.h>
#include <WiFiManager.h>
#include <OtaUpdateHelper.h>
#include <configManager.h>
#include <timeSync.h>
#include <TZ.h>
#include <sntp.h>
#include "dashboard.h"
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

struct task {
	unsigned long rate;
	unsigned long previous;
};

task taskA = {.rate = 333, .previous = 0};

void updateDashboard() {
//	if (taskA.previous == 0 || (millis() - taskA.previous > taskA.rate)) {
//		taskA.previous = millis();
//
//		String stringOne = "Apples";
//		stringOne.toCharArray(dash.data.projectName, 32);
//
//		dash.data.dummyInt++;
//		dash.data.inputInt++;
//
//		dash.data.dummyFloat = sin((float) millis() / 5000);
//
//		if (dash.data.inputBool)
//			dash.data.dummyBool = true;
//		else
//			dash.data.dummyBool = false;
//	}
}

void setup() {
	setupSerial();

	Serial.printf("==[ global setup ]==============\n");

	initGlobals();

	wiFiManager.begin(getConfigManager()->getEepromData().getStoredData().getConfigData().projectName);

//	timeSync.begin(TZ_Europe_Warsaw);
//	timeSync.waitForSyncResult(10000);

//	dash.begin(1000);
}

// FIXME: move loop() to Runner.cpp

void loop() {
	static IRunner *runner = getRunner();

	wiFiManager.loop();
//	otaUpdateHelper.loop();
//	dash.loop();
//	updateDashboard();
//	delay(1000);
//	showNow();

	runner->getScheduler()->execute();
}
