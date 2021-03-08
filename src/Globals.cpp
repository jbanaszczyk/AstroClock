#include "Globals.h"
#include <LittleFS.h>
#include <webServer.h>
#include <WiFiManager.h>

void initGlobals() {

	LittleFS.begin();

	auto runner = getRunner();

	auto configManager = getConfigManager();
	auto wiFiManger = getWiFiManager(getConfigManager()->getEepromData().getStoredData().getConfigData().projectName);

//	auto wiFiMonitor = getWiFiMonitor();

	auto commandProcessor = getCommandProcessor(new SerialCommands());

	GUI.begin();

	auto scheduler = runner->getScheduler();
	configManager->addScheduler(scheduler);
	wiFiManger->addScheduler(scheduler);
	commandProcessor->addScheduler(scheduler);
	GUI.addScheduler(scheduler);
}
