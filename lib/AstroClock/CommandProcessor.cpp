#include <Arduino.h>
#include "CommandProcessor.h"
#include <SerialCommands.h>
#include <ESP8266WiFi.h>
#include "WiFiManager.h"

/*
void doSthWithArgs(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->print(PSTR("Got command\n"));
		decltype(sender->Next()) argument;
		while ((argument = sender->Next()) != nullptr) {
			stream->printf_P(PSTR("\targument: %s\n"), argument);
		}
		stream->print(PSTR("=================\n"));
	}
}
*/

void CommandProcessor::wifiStaForget(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->print(PSTR("OK\n"));
	}
	getWiFiManager(nullptr)->prepareWiFi_STA_forget();
}

void CommandProcessor::wifiStaConnect(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->print(PSTR("OK\n"));
	}
	auto ssid = sender->Next();
	auto password = sender->Next();
	getWiFiManager(nullptr)->prepareWiFi_STA(ssid, password);
}

void CommandProcessor::wifiApPsk(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->print(PSTR("OK\n"));
	}
	auto password = sender->Next();
	getWiFiManager(nullptr)->prepareWiFi_AP_PSK(password);
}

void CommandProcessor::showStatus(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->printf_P(PSTR("Heap free:   %d\n"), system_get_free_heap_size());
		stream->print(PSTR("==[ WiFi ]=============\n"));
		stream->printf_P(PSTR("Hostname:    %s\n"), WiFi.hostname().c_str());
		stream->printf_P(PSTR("WiFi status: %d\n"), WiFi.status());
		stream->printf_P(PSTR("WiFi mode:   %d\n"), WiFi.getMode());

		if (WiFi.getMode() & WIFI_STA) {
			stream->print(PSTR("==[ STAtion ]==========\n"));
			stream->printf_P(PSTR("SSID:        %s\n"), WiFi.SSID().c_str());
			stream->printf_P(PSTR("IP:          %s\n"), WiFi.localIP().toString().c_str());
			stream->printf_P(PSTR("Subnet:      %s\n"), WiFi.subnetMask().toString().c_str());
			stream->printf_P(PSTR("Gateway:     %s\n"), WiFi.gatewayIP().toString().c_str());
			stream->printf_P(PSTR("DNS:         %s\n"), WiFi.dnsIP().toString().c_str());
			stream->printf_P(PSTR("DHCP:        %d\n"), wifi_station_dhcpc_status());
			stream->printf_P(PSTR("%s\n"), WiFi.getPersistent() ? "Persistent" : "Volatile\n");
			stream->printf_P(PSTR("%s\n"), WiFi.getAutoReconnect() ? "Auto reconnect" : "Dont reconnect");
		}

		if (WiFi.getMode() & WIFI_AP) {
			stream->print(PSTR("==[ AP ]===============\n"));
			stream->printf_P(PSTR("SSID:        %s\n"), WiFi.softAPSSID().c_str());
			stream->printf_P(PSTR("IP:          %s\n"), WiFi.softAPIP().toString().c_str());
		}

		stream->println("==[ Credentials ]======");
		stream->printf_P(PSTR("PSK:         %s\n"), WiFi.psk().c_str());
		stream->printf_P(PSTR("AP PSK:      %s\n"), WiFi.softAPPSK().c_str());
	}
}

void CommandProcessor::doHelp(SerialCommands *sender) {
	sender->printHelp();
}

void CommandProcessor::doNothing(SerialCommands *sender) {
	if (auto stream = sender->getStream()) {
		stream->println();
	}
}

CommandProcessor::CommandProcessor(SerialCommands *serialCommands) :
		serialCommands(serialCommands) {

	serialCommands->AddCommand(PSTR("wifi status"), PSTR("Show wifi status"), showStatus);
	serialCommands->AddCommand(PSTR("wifi off"), PSTR("Forget wifi credentials"), wifiStaForget);
	serialCommands->AddCommand(PSTR("wifi on"), PSTR("Connect wifi: [ssid] [password] "), wifiStaConnect);
	serialCommands->AddCommand(PSTR("wifi ap"), PSTR("Change AP password: [password] "), wifiApPsk);

	serialCommands->AddCommand(PSTR("help"), PSTR("Show this help"), doHelp);

	serialCommands->AddCommand(' ', nullptr, doNothing);

	serialCommands->getStream()->print(PSTR("Type 'help' to get help\n"));
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
