#include "Globals.h"
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <NTPSync.h>
#include <chrono>

std::unique_ptr<char[]> timeStr(time_t now) {
	struct tm *info = localtime(&now);
	static const int StrftimeBufferSize = 80;
	std::unique_ptr<char[]> buffer(new char[StrftimeBufferSize]);
	strftime(buffer.get(), StrftimeBufferSize, "%Y.%m.%d %H.%M.%S", info);
	return buffer;
}

void initGlobals() {

	LittleFS.begin();

	auto runner = getRunner();

	auto configManager = getConfigManager();
	auto wiFiManger = getWiFiManager(getConfigManager()->getEepromData().getStoredData().getConfigData().projectName);

	configTime(
			getConfigManager()->getEepromData().getStoredData().getConfigData().timeZone,
			"0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org");

	auto ntpSync = getNTPSync();

	auto commandProcessor = getCommandProcessor(new SerialCommands());
	auto webServer = getWebServer();
//	auto wiFiMonitor = getWiFiMonitor();

	auto scheduler = runner->getScheduler();
	configManager->addScheduler(scheduler);
	wiFiManger->addScheduler(scheduler);
	commandProcessor->addScheduler(scheduler);
	webServer->addScheduler(scheduler);

	Serial.printf("Current local time: %s\n", timeStr(ntpSync->getNow()).get());

	while (!ntpSync->isSynced()) {
		delay(1000);
		Serial.printf("Current local time: %s\n", timeStr(ntpSync->getNow()).get());

		auto elapsed_time_ms = std::chrono::duration<long>(ntpSync->getNow() - ntpSync->getStartTime()).count();
		Serial.printf("_______ %ld\n", elapsed_time_ms);
	}
	delay(12345);
	auto elapsed_time_ms = std::chrono::duration<double, std::milli>(ntpSync->getNow() - ntpSync->getStartTime()).count();
	Serial.printf("======= %f\n", elapsed_time_ms);

}
