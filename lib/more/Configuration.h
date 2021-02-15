#pragma once

#include <Arduino.h>
#include <EEPROM_Rotate.h>
#include "Configuration_data.h"

class IConfiguration {
public:
	virtual ~IConfiguration() = default;
};

class Configuration : public IConfiguration {
public:
	explicit Configuration(EEPROM_Rotate &eeprom);

	const Configuration_data &getData() const;

	bool commit(EEPROM_Rotate &eeprom);

private:
	Configuration_data data{};
	EEPROM_Rotate &eeprom;
	uint16_t reservedAreaSize;

	uint16_t getReservedAreaSize(EEPROM_Rotate &eeprom) const;
};

EEPROM_Rotate &get_EEPROM_Rotate();

IConfiguration &getConfiguration(EEPROM_Rotate &eepromRotate);
