#include <Arduino.h>
#include <LittleFS.h>

#include <WiFiManager.h>
#include <webServer.h>
#include <OtaUpdateHelper.h>
#include <HTTPRequest.h>
#include <configManager.h>
#include <timeSync.h>

String macToString(const unsigned char *mac) {
	char buf[20];
	snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
	         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return String(buf);
}

static void onStationModeGotIP(const WiFiEventStationModeGotIP &evt) {
	Serial.println("==[ onStationModeGotIP ]========");
	Serial.println(evt.ip);
	Serial.println(evt.mask);
	Serial.println(evt.gw);
}

static void onStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged &evt) {
	Serial.println("==[ onStationModeAuthModeChanged ]========");
	Serial.println(evt.oldMode);
	Serial.println(evt.newMode);
}

static void onStationModeConnected(const WiFiEventStationModeConnected &evt) {
	Serial.println("==[ onStationModeConnected ]========");
	Serial.println(evt.ssid);
	Serial.println(evt.channel);
}

static void onStationModeDisconnected(const WiFiEventStationModeDisconnected &evt) {
	Serial.println("==[ onStationModeDisconnected ]========");
	Serial.println(evt.ssid);
	Serial.println(evt.reason);
}

static void onStationModeDHCPTimeout() {
	Serial.println("==[ onStationModeDHCPTimeout ]========");
}

static void onWiFiModeChange(const WiFiEventModeChange evt) {
	Serial.println("==[ onWiFiModeChange ]========");
	Serial.println(evt.oldMode);
	Serial.println(evt.newMode);
}

static void onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected &evt) {
	Serial.println("==[ onSoftAPModeStationConnected ]========");
	Serial.println(macToString(evt.mac));
}

static void onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected &evt) {
	Serial.println("==[ onSoftAPModeStationDisconnected ]========");
	Serial.println(macToString(evt.mac));
}


void setup() {
	Serial.begin(115200);
	while (!Serial) {}
	Serial.println();

	LittleFS.begin();
	GUI.begin();
	configManager.begin();
	WiFiManager.begin(configManager.data.projectName);
	timeSync.begin();

	static WiFiEventHandler handlerStationModeGotIP = WiFi.onStationModeGotIP(onStationModeGotIP);
	static WiFiEventHandler handlerStationModeAuthModeChanged = WiFi.onStationModeAuthModeChanged(onStationModeAuthModeChanged);
	static WiFiEventHandler handlerStationModeConnected = WiFi.onStationModeConnected(onStationModeConnected);
	static WiFiEventHandler handlerStationModeDHCPTimeout = WiFi.onStationModeDHCPTimeout(onStationModeDHCPTimeout);
	static WiFiEventHandler handlerStationModeDisconnected = WiFi.onStationModeDisconnected(onStationModeDisconnected);
	static WiFiEventHandler handlerWiFiModeChange = WiFi.onWiFiModeChange(onWiFiModeChange);

	static WiFiEventHandler handlerSoftAPModeStationConnected = WiFi.onSoftAPModeStationConnected(onSoftAPModeStationConnected);
	static WiFiEventHandler handlerSoftAPModeStationDisconnected = WiFi.onSoftAPModeStationDisconnected(onSoftAPModeStationDisconnected);


}


void getExample() {
	Serial.println("Before httpRequest");
	HTTPRequest httpRequest("https://www.google.com");
//		HTTPRequest httpRequest("http://www.google.com");
//		HTTPRequest httpRequest("https://api.ipgeolocation.io/timezone?apiKey=a8d4b9360d214b32bf057aaf1a6907ec");

	auto result = httpRequest.GET();
	Serial.printf("HTTP status: %d\n", result);

	while (httpRequest.busy()) {
		if (httpRequest.available()) {
			Serial.write(httpRequest.read());
		} else {
			delay(1);
		}
	}

	httpRequest.end();
	Serial.println("++++++++++++");
}

void dumpWiFIStatus() {
	Serial.printf("Name:    %s\n", WiFi.hostname().c_str());
	Serial.printf("Status:  %d\n", WiFi.status());
	Serial.printf("Mode:    %d\n", WiFi.getMode());
	Serial.printf("SSID:    %s\n", WiFi.SSID().c_str());
	Serial.printf("IP:      %s\n", WiFi.localIP().toString().c_str());
	Serial.printf("Subnet:  %s\n", WiFi.subnetMask().toString().c_str());
	Serial.printf("Gateway: %s\n", WiFi.gatewayIP().toString().c_str());
	Serial.printf("DNS:     %s\n", WiFi.dnsIP().toString().c_str());
	Serial.printf("DHCP:    %d\n", wifi_station_dhcpc_status());
	WiFi.getPersistent() ? Serial.printf("Persistent\n") : Serial.printf("Volatile\n");
	WiFi.getAutoReconnect() ? Serial.printf("Auto reconnect\n") : Serial.printf("Dont reconnect\n");
	Serial.println("____________________");
	Serial.printf("PSK:     %s\n", WiFi.psk().c_str());
}

void disconnect() {
	Serial.printf("Disconnect\n");
	WiFi.disconnect();
}

void reconnect() {
	Serial.printf("Connect\n");

	WiFi.config(
			IPAddress(),
			IPAddress(),
			IPAddress()
	);


	WiFi.begin("GolemXIV", "DuPa.9736");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.printf("\nStatus %d ", WiFi.status());
	Serial.println(WiFi.localIP());
}

void alt_reconnect() {
	Serial.printf("Connect\n");

	WiFi.config(
			IPAddress(192, 168, 1, 200),
			IPAddress(192, 168, 1, 1),
			IPAddress(255, 255, 0, 0),
			IPAddress(192, 168, 1, 1)
	);
	WiFi.begin("GolemXIV", "DuPa.9736");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.printf("\nStatus %d ", WiFi.status());
	Serial.println(WiFi.localIP());
}

void loop() {
	WiFiManager.loop();
	otaUpdateHelper.loop();

	static auto initFreeHeap = ESP.getFreeHeap();


	if (Serial.available()) {

		char c = Serial.read();

		switch (c) {
			case 'g': {
				getExample();
				break;
			}
			case '?': {
				Serial.printf("Memory: %d %d\n", ESP.getFreeHeap(), initFreeHeap);
				dumpWiFIStatus();
				break;
			}
			case '+': {
				reconnect();
				break;
			}
			case '-': {
				disconnect();
				break;
			}
			case 'h': {
				alt_reconnect();
				break;
			}
			case '\n':
			case '\r':
			case ' ': {
				Serial.println();
				break;
			}
			default: {
				Serial.printf("Was read %c %x\n", c, c);
			}
		}
	}
}
