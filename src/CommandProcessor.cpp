#include <Arduino.h>
#include "CommandProcessor.h"
#include <SerialCommands.h>
#include <ESP8266WiFi.h>
#include "Globals.h"

/* FIXME To Be Deleted

void doSthWithArgs(SerialCommands *sender) {
	Stream *stream = stream;
	stream->printf("Got command\n");
	decltype(sender->Next()) argument;
	while ((argument = sender->Next()) != nullptr) {
		stream->printf("\targument: %s\n", argument);
	}
	stream->printf("=================\n");
}

 */

static const char *status2string(bool argument) {
	return argument
	       ? "OK"
	       : "Fail";
}

#ifdef DEVELOPMENT

void CommandProcessor::wifiStaDisconnect(SerialCommands *sender) {
	auto stream = sender->getStream();
	if (stream != nullptr) {
		stream->printf("wifiStaDisconnect: ");
	}
	auto result = WiFi.disconnect();
	if (stream != nullptr) {
		stream->printf("%s\n", status2string(result));
	}
}

void CommandProcessor::wifiStaConnect(SerialCommands *sender) {
	auto stream = sender->getStream();
	if (stream != nullptr) {
		stream->printf("wifiStaConnect: ");
	}
	WiFi.config(IPAddress(), IPAddress(), IPAddress());

	// FIXME remove credentials
	WiFi.begin("GolemXIV", "DuPa.9736");
	WiFi.waitForConnectResult();
	auto result = WiFi.isConnected();
	if (stream != nullptr) {
		stream->printf("%s\n", status2string(result));
	}
}

#endif

void CommandProcessor::showStatus(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->printf("Heap free:   %d\n", system_get_free_heap_size());
		stream->println("==[ WiFi ]=============");
		stream->printf("Hostname:    %s\n", WiFi.hostname().c_str());
		stream->printf("WiFi status: %d\n", WiFi.status());
		stream->printf("WiFi mode:   %d\n", WiFi.getMode());

		if (WiFi.getMode() & WIFI_STA) {
			stream->println("==[ STAtion ]==========");
			stream->printf("SSID:        %s\n", WiFi.SSID().c_str());
			stream->printf("IP:          %s\n", WiFi.localIP().toString().c_str());
			stream->printf("Subnet:      %s\n", WiFi.subnetMask().toString().c_str());
			stream->printf("Gateway:     %s\n", WiFi.gatewayIP().toString().c_str());
			stream->printf("DNS:         %s\n", WiFi.dnsIP().toString().c_str());
			stream->printf("DHCP:        %d\n", wifi_station_dhcpc_status());
			stream->printf("%s\n", WiFi.getPersistent() ? "Persistent" : "Volatile\n");
			stream->printf("%s\n", WiFi.getAutoReconnect() ? "Auto reconnect" : "Dont reconnect");
		}

		if (WiFi.getMode() & WIFI_AP) {
			stream->println("==[ AP ]===============");
			stream->printf("SSID:        %s\n", WiFi.softAPSSID().c_str());
			stream->printf("IP:          %s\n", WiFi.softAPIP().toString().c_str());
		}

		stream->println("==[ Credentials ]======");
		stream->printf("PSK:         %s\n", WiFi.psk().c_str());
		stream->printf("AP PSK:      %s\n", WiFi.softAPPSK().c_str());
		stream->println();
	}
}

void CommandProcessor::doHelp(SerialCommands *sender) {
	sender->printHelp();
}

void CommandProcessor::doNothing(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->printf("\n");
	}
}

void CommandProcessor::doConf(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		auto config = getConfigManager()->getMutableEepromData()->getStoredData().getConfigData();
		stream->printf("config projectName %s\n", config.projectName);
		stream->printf("config %d\n", config.dummyInt);
	}
}

void CommandProcessor::doSave(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		auto config = getConfigManager()->getMutableEepromData()->getStoredData().getConfigData();
		stream->printf("config projectName %s\n", config.projectName);
		stream->printf("config %d\n", config.dummyInt);
		getConfigManager()->setDirty();
	}
}

CommandProcessor::CommandProcessor(SerialCommands *serialCommands) :
		serialCommands(serialCommands) {
#ifdef DEVELOPMENT
	serialCommands->AddCommand('-', "WiFi STAtion disconnect", wifiStaDisconnect);
	serialCommands->AddCommand('+', "WiFi STAtion connect", wifiStaConnect);
#endif
	serialCommands->AddCommand('x', nullptr, doConf);
	serialCommands->AddCommand('q', nullptr, doSave);
	serialCommands->AddCommand(' ', nullptr, doNothing);
	serialCommands->AddCommand('?', "show system status", showStatus);
	serialCommands->AddCommand("help", "Show help", doHelp);
	serialCommands->getStream()->printf("Type help to get help\n");
	createLoopTask();
}

void CommandProcessor::createLoopTask() {
	loopTask = new Task(
			33,
			-1,
			[this]() -> void {
				if (serialCommands != nullptr) {
					serialCommands->ReadSerial();
				}
			},
			getRunner()->getScheduler(),
			true);
}

void CommandProcessor::setStream(Stream *stream) {
	serialCommands->setStream(stream);
	if (stream != nullptr) {
		loopTask->enable();
	} else {
		loopTask->disable();
	}
}

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands) {
	static CommandProcessor commandProcessor = CommandProcessor(serialCommands);
	return &commandProcessor;
}
