
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

#include <TZ.h>
#include <sntp.h>
#include "dashboard.h"
#include "Globals.h"
#include "utilities.h"
#include <Runner.h>

void setupSerial(unsigned long baud = 74880) {
	Serial.begin(baud);
	while (!Serial) {}
	Serial.println();
}

//Task tShowTimeOnSync(
//		0,
//		1,
//		[]() -> void {
//			auto *ntpSync = getNTPSync();
//			Serial.printf("[NtpSync] Current local time: %s\n", ntpSync->timeStr(ntpSync->getNow()).get());
//			ntpSync->clrSynced();
//		},
//		nullptr,
//		true
//);

void updateDashboard() {
	String stringOne = "Apples";
	auto dashboardInput = getDashboard()->getDashboardData();
	auto dashboardOutput = getDashboard()->getMutualDashboardData();
	stringOne.toCharArray(dashboardOutput->projectName, sizeof(dashboardInput.projectName));
	dashboardOutput->dummyInt = dashboardOutput->inputInt++;
	dashboardOutput->dummyFloat = sin((float) millis() / 5000);
	dashboardOutput->dummyBool = dashboardInput.inputBool;
}

Task tUpdateDashboard(
		333,
		-1,
		[]() -> void {
			updateDashboard();
		},
		nullptr,
		true
);

void setup() {
	setupSerial();

	Serial.printf("==[ global setup ]==============\n");

	initGlobals();


//	getRunner()->getScheduler()->addTask(tShowTimeOnSync);
//	getNTPSync()->addSyncCallback([]() -> void { tShowTimeOnSync.restart(); });

	getRunner()->getScheduler()->addTask(tUpdateDashboard);
	tUpdateDashboard.enable();

	Serial.printf("==[ global setup done ]=========\n");
}
