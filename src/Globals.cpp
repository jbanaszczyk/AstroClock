#include "Globals.h"
#include <LittleFS.h>
#include <webServer.h>

void initGlobals() {
	auto runner = getRunner();
//	auto wiFiMonitor = getWiFiMonitor();
	auto commandProcessor = getCommandProcessor(new SerialCommands());
	auto configManager = getConfigManager();

	LittleFS.begin();
	GUI.begin();

	auto scheduler = runner->getScheduler();
	commandProcessor->addScheduler(scheduler);
	configManager->addScheduler(scheduler);
	GUI.addScheduler(scheduler);
}
