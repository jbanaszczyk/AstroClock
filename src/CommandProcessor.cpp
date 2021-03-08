#include <Arduino.h>
#include "CommandProcessor.h"
#include <SerialCommands.h>
#include <ESP8266WiFi.h>
#include "Globals.h"
#include "WiFiManager.h"

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

void CommandProcessor::wifiStaForget(SerialCommands *sender) {
	getWiFiManager(nullptr)->prepareWiFi_STA_forget();
}

void CommandProcessor::wifiStaConnect(SerialCommands *sender) {
	getWiFiManager(nullptr)->prepareWiFi_STA("GolemXIV", "DuPa.9736");
}

void CommandProcessor::wifiStaConnect_wrong(SerialCommands *sender) {
	getWiFiManager(nullptr)->prepareWiFi_STA("UnknownSSID", "wrongPass");
}

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

CommandProcessor::CommandProcessor(SerialCommands *serialCommands) :
		serialCommands(serialCommands) {
	serialCommands->AddCommand('-', "WiFi STAtion disconnect", wifiStaForget);
	serialCommands->AddCommand('+', "WiFi STAtion connect with DHCP", wifiStaConnect);
	serialCommands->AddCommand('$', "WiFi STAtion connect wrong SSID", wifiStaConnect_wrong);
	serialCommands->AddCommand(' ', nullptr, doNothing);
	serialCommands->AddCommand('?', "show system status", showStatus);
	serialCommands->AddCommand("help", "Show help", doHelp);
	serialCommands->getStream()->printf("Type help to get help\n");
}

void CommandProcessor::addScheduler(Scheduler *scheduler) {
	if (scheduler != nullptr) {
		loopTask = new Task(
				SerialPortReadRepeatRate,
				-1,
				[this]() -> void {
					if (serialCommands != nullptr) {
						serialCommands->ReadSerial();
					}
				},
				scheduler,
				true);
	}
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
