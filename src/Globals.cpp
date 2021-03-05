#include "Globals.h"
#include <LittleFS.h>
#include <webServer.h>

void initGlobals() {
	auto runner = getRunner();
//	auto wiFiMonitor = getWiFiMonitor();
	auto commandProcessor = getCommandProcessor(new SerialCommands());
	auto configManager = getConfigManager();
	configManager->addScheduler( runner->getScheduler());

	LittleFS.begin();
	GUI.begin();
}
