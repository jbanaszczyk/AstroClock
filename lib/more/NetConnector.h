#pragma once

#include <Arduino.h>

class INetConnector {
public:
	virtual ~INetConnector() = default;
};

class NetConnector : public INetConnector {
public:
	explicit NetConnector();
};

INetConnector &getNetConnector();

// bobrowiecka jutro 16:45 ulrich
// 22 3381922


