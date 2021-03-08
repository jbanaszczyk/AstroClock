#include "Globals.h"
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFiManager.h>

void initGlobals() {

	LittleFS.begin();

	auto runner = getRunner();

	auto configManager = getConfigManager();
	auto wiFiManger = getWiFiManager(getConfigManager()->getEepromData().getStoredData().getConfigData().projectName);
	auto commandProcessor = getCommandProcessor(new SerialCommands());
	auto webServer = getWebServer();
//	auto wiFiMonitor = getWiFiMonitor();

	auto scheduler = runner->getScheduler();
	configManager->addScheduler(scheduler);
	wiFiManger->addScheduler(scheduler);
	commandProcessor->addScheduler(scheduler);
	webServer->addScheduler(scheduler);
}
