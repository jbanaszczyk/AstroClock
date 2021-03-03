#include "CommandProcessor.h"

#include <Arduino.h>
#include <SerialCommands.h>

#include <ESP8266WiFi.h>


/* FIXME To Be Deleted


void doSthWithArgs(SerialCommands *sender) {
	Stream *stream = sender->getStream();
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
	sender->getStream()->printf("wifiStaDisconnect: ");
	auto result = WiFi.disconnect();
	sender->getStream()->printf("%s\n", status2string(result));
}

void CommandProcessor::wifiStaConnect(SerialCommands *sender) {
	sender->getStream()->printf("wifiStaConnect: ");

	WiFi.config(IPAddress(), IPAddress(), IPAddress());

	// FIXME remove credentials
	WiFi.begin("GolemXIV", "DuPa.9736");
	WiFi.waitForConnectResult();
	auto result = WiFi.isConnected();
	sender->getStream()->printf("%s\n", status2string(result));
}

#endif

void CommandProcessor::showStatus(SerialCommands *sender) {
	sender->getStream()->printf("Hostname:    %s\n", WiFi.hostname().c_str());
	sender->getStream()->printf("WiFi status: %d\n", WiFi.status());
	sender->getStream()->printf("WiFi mode:   %d\n", WiFi.getMode());

	if (WiFi.getMode() & WIFI_STA) {
		sender->getStream()->println("==[ STAtion ]==========");
		sender->getStream()->printf("SSID:        %s\n", WiFi.SSID().c_str());
		sender->getStream()->printf("IP:          %s\n", WiFi.localIP().toString().c_str());
		sender->getStream()->printf("Subnet:      %s\n", WiFi.subnetMask().toString().c_str());
		sender->getStream()->printf("Gateway:     %s\n", WiFi.gatewayIP().toString().c_str());
		sender->getStream()->printf("DNS:         %s\n", WiFi.dnsIP().toString().c_str());
		sender->getStream()->printf("DHCP:        %d\n", wifi_station_dhcpc_status());
		sender->getStream()->printf("  %s\n", WiFi.getPersistent() ? "Persistent" : "Volatile\n");
		sender->getStream()->printf("  %s\n", WiFi.getAutoReconnect() ? "Auto reconnect" : "Dont reconnect");
	}

	if (WiFi.getMode() & WIFI_AP) {
		sender->getStream()->println("==[ AP ]===============");
		sender->getStream()->printf("SSID:        %s\n", WiFi.softAPSSID().c_str());
		sender->getStream()->printf("IP:          %s\n", WiFi.softAPIP().toString().c_str());
	}

	sender->getStream()->println("____________________");
	sender->getStream()->printf("PSK:         %s\n", WiFi.psk().c_str());
	sender->getStream()->printf("AP PSK:      %s\n", WiFi.softAPPSK().c_str());
	sender->getStream()->println();
}

void CommandProcessor::doHelp(SerialCommands *sender) {
	sender->printHelp();
}

CommandProcessor::CommandProcessor(SerialCommands *serialCommands) :
		serialCommands(serialCommands) {

#ifdef DEVELOPMENT
	serialCommands->AddCommand('-', "WiFi STAtion disconnect", wifiStaDisconnect);
	serialCommands->AddCommand('+', "WiFi STAtion connect", wifiStaConnect);
#endif
	serialCommands->AddCommand('?', "show system status", showStatus);
	serialCommands->AddCommand("help", "Show help", doHelp);
	serialCommands->getStream()->printf("Type help to get help\n");
}

void CommandProcessor::loop() {
	if (serialCommands != nullptr) {
		serialCommands->ReadSerial();
	}
}

void CommandProcessor::setStream(Stream *stream) {
	serialCommands->setStream(stream);
}

ICommandProcessor *getCommandProcessor(SerialCommands *serialCommands) {
	static CommandProcessor commandProcessor = CommandProcessor(serialCommands);
	return &commandProcessor;
}
