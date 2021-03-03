#include <algorithm>

#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

class IWiFiMonitor {
public:
	virtual ~IWiFiMonitor() = default;
};

class WiFiMonitor : public IWiFiMonitor {
public:
	explicit WiFiMonitor();

	void onStationModeGotIP(const WiFiEventStationModeGotIP &evt);

	void onStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged &evt);

	void onStationModeConnected(const WiFiEventStationModeConnected &evt);

	void onStationModeDisconnected(const WiFiEventStationModeDisconnected &evt);

	void onStationModeDHCPTimeout();

	void onWiFiModeChange(const WiFiEventModeChange &evt);

	void onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected &evt);

	void onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected &evt);

private:
	WiFiEventHandler handlerStationModeGotIP;
	WiFiEventHandler handlerStationModeAuthModeChanged;
	WiFiEventHandler handlerStationModeConnected;
	WiFiEventHandler handlerStationModeDHCPTimeout;
	WiFiEventHandler handlerStationModeDisconnected;
	WiFiEventHandler handlerWiFiModeChange;
	WiFiEventHandler handlerSoftAPModeStationConnected;
	WiFiEventHandler handlerSoftAPModeStationDisconnected;

	static String macToString(const unsigned char *mac);
};

IWiFiMonitor *getWiFiMonitor();
