#include "Globals.h"
#include <LittleFS.h>
#include <webServer.h>

void initGlobals() {
	getRunner();
//	getWiFiMonitor();
	getCommandProcessor(new SerialCommands());
	getConfigManager();

	LittleFS.begin();
	GUI.begin();
}
