#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

class IWiFiMonitor {
public:
	virtual ~IWiFiMonitor() = default;
};

IWiFiMonitor *getWiFiMonitor();

class WiFiMonitor : public IWiFiMonitor {
public:
	explicit WiFiMonitor();

	void onStationModeConnected(const WiFiEventStationModeConnected &evt);

//	void onStationModeDisconnected(const WiFiEventStationModeDisconnected &evt);

//	void onStationModeGotIP(const WiFiEventStationModeGotIP &evt);

//	void onWiFiModeChange(const WiFiEventModeChange &evt);

	void onStationModeAuthModeChanged(const WiFiEventStationModeAuthModeChanged &evt);

	void onStationModeDHCPTimeout();

	void onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected &evt);

	void onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected &evt);

private:
	WiFiEventHandler handlerStationModeConnected;
//	WiFiEventHandler handlerStationModeDisconnected;
//	WiFiEventHandler handlerStationModeGotIP;
//	WiFiEventHandler handlerWiFiModeChange;
	WiFiEventHandler handlerStationModeAuthModeChanged;
	WiFiEventHandler handlerStationModeDHCPTimeout;
	WiFiEventHandler handlerSoftAPModeStationConnected;
	WiFiEventHandler handlerSoftAPModeStationDisconnected;

	static String macToString(const unsigned char *mac);
};
