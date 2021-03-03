#include "WiFiMonitor.h"

WiFiMonitor::WiFiMonitor() {
	handlerStationModeGotIP = WiFi.onStationModeGotIP(std::bind(&WiFiMonitor::onStationModeGotIP, this, std::placeholders::_1));
	handlerStationModeAuthModeChanged = WiFi.onStationModeAuthModeChanged(std::bind(&WiFiMonitor::onStationModeAuthModeChanged, this, std::placeholders::_1));
	handlerStationModeConnected = WiFi.onStationModeConnected(std::bind(&WiFiMonitor::onStationModeConnected, this, std::placeholders::_1));
	handlerStationModeDHCPTimeout = WiFi.onStationModeDHCPTimeout(std::bind(&WiFiMonitor::onStationModeDHCPTimeout, this));
	handlerStationModeDisconnected = WiFi.onStationModeDisconnected(std::bind(&WiFiMonitor::onStationModeDisconnected, this, std::placeholders::_1));
	handlerWiFiModeChange = WiFi.onWiFiModeChange(std::bind(&WiFiMonitor::onWiFiModeChange, this, std::placeholders::_1));
	handlerSoftAPModeStationConnected = WiFi.onSoftAPModeStationConnected(std::bind(&WiFiMonitor::onSoftAPModeStationConnected, this, std::placeholders::_1));
	handlerSoftAPModeStationDisconnected = WiFi.onSoftAPModeStationDisconnected(std::bind(&WiFiMonitor::onSoftAPModeStationDisconnected, this, std::placeholders::_1));
}

String WiFiMonitor::macToString(const unsigned char *mac) {
	char buf[20];
	snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
	         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	return String(buf);
}

void WiFiMonitor::onStationModeGotIP(const WiFiEventStationModeGotIP &evt) {
	Serial.println("==[ onStationModeGotIP ]========");
	Serial.println(evt.ip);
	Serial.println(evt.mask);
	Serial.println(evt.gw);
}

void WiFiMonitor::onStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged &evt) {
	Serial.println("==[ onStationModeAuthModeChanged ]========");
	Serial.println(evt.oldMode);
	Serial.println(evt.newMode);
}

void WiFiMonitor::onStationModeConnected(const WiFiEventStationModeConnected &evt) {
	Serial.println("==[ onStationModeConnected ]========");
	Serial.println(evt.ssid);
	Serial.println(evt.channel);
}

void WiFiMonitor::onStationModeDisconnected(const WiFiEventStationModeDisconnected &evt) {
	Serial.println("==[ onStationModeDisconnected ]========");
	Serial.println(evt.ssid);
	Serial.println(evt.reason);
}

void WiFiMonitor::onStationModeDHCPTimeout() {
	Serial.println("==[ onStationModeDHCPTimeout ]========");
}

void WiFiMonitor::onWiFiModeChange(const WiFiEventModeChange &evt) {
	Serial.println("==[ onWiFiModeChange ]========");
	Serial.println(evt.oldMode);
	Serial.println(evt.newMode);
}

void WiFiMonitor::onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected &evt) {
	Serial.println("==[ onSoftAPModeStationConnected ]========");
	Serial.println(macToString(evt.mac));
}

void WiFiMonitor::onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected &evt) {
	Serial.println("==[ onSoftAPModeStationDisconnected ]========");
	Serial.println(macToString(evt.mac));
}

IWiFiMonitor *getWiFiMonitor() {
	static WiFiMonitor wiFiMonitor = WiFiMonitor();
	return &wiFiMonitor;
}
