
#include <Arduino.h>

#include "NetConnector.h"

#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>


#if defined(DEBUG_NET_CONNECTOR) && defined(DEBUG_ESP_PORT)
#define LOG_WIFI(...) DEBUG_ESP_PORT.printf( "[WIFI] " __VA_ARGS__ )
#else
#define LOG_WIFI(...)
#endif


INetConnector &getNetConnector() {
	static NetConnector netConnector;
	return netConnector;
}

NetConnector::NetConnector() {

//	enum class WiFiStatus {
//		WL_IDLE = 0,
//		WL_NO_SSID_AVAIL = 1,
//		WL_SCAN_COMPLETED = 2,
//		WL_CONNECTED = 3,
//		WL_CONNECT_FAILED = 4,
//		WL_CONNECTION_LOST = 5,
//		WL_DISCONNECTED = 6,
//	};
//
	LOG_WIFI("Start\n");
//
//	char ssid[] = "myNetwork";          //  your network SSID (name)
//	char pass[] = "myPassword";   // your network password
//
//	WiFiStatus wifiStatus = WiFiStatus::WL_IDLE;


	WiFi.mode(WIFI_AP);
	WiFi.persistent()
//	WiFi.mode(WIFI_STA);
//	WiFi.begin("GolemXIV", "DuPa.9736_");

	LOG_WIFI("Done\n");




}
