#include "Globals.h"
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <NTPSync.h>
#include <Dashboard.h>

void initGlobals() {

	LittleFS.begin();

	auto runner = getRunner();

	auto configManager = getConfigManager();
	auto wiFiManger = getWiFiManager(getConfigManager()->getEepromData().getStoredData().getConfigData().projectName);

	configTime(
			getConfigManager()->getEepromData().getStoredData().getConfigData().timeZone,
			"0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org");

	getNTPSync();

	auto commandProcessor = getCommandProcessor(new SerialCommands());
	auto webServer = getWebServer();
	auto dashboard = getDashboard();

//	auto wiFiMonitor = getWiFiMonitor();

	auto scheduler = runner->getScheduler();
	configManager->addScheduler(scheduler);
	wiFiManger->addScheduler(scheduler);
	commandProcessor->addScheduler(scheduler);
	webServer->addScheduler(scheduler);
	dashboard->addScheduler(getRunner()->getScheduler());
}
