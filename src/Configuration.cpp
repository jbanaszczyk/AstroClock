#include "Configuration.h"

#ifdef DEBUG_CONFIGURATION_PORT
#define DEBUG_CONFIGURATION(...) DEBUG_CONFIGURATION_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_CONFIGURATION(...)
#endif

EEPROM_Rotate &get_EEPROM_Rotate() {
	static EEPROM_Rotate eepromRotate;
	return eepromRotate;
}

IConfiguration &getConfiguration(EEPROM_Rotate &eepromRotate) {
	static Configuration configuration(eepromRotate);
	return configuration;
}

uint16_t Configuration::getReservedAreaSize(EEPROM_Rotate &eeprom) const {
	decltype(this->reservedAreaSize) reservedAreaSize = 3;
	const decltype(reservedAreaSize) maxReservedAreaSize = 16;
	while (reservedAreaSize < maxReservedAreaSize) {
		if (eeprom.offset(static_cast<uint16_t>(SPI_FLASH_SEC_SIZE - reservedAreaSize))) {
			break;
		}
		++reservedAreaSize;
	}
	eeprom.offset(0);
	if (reservedAreaSize == maxReservedAreaSize) {
		Serial.printf("!!! [EEPROM] Unable to find EEPROM reservedAreaSize. Assuming %u\n", reservedAreaSize);
	}
	return reservedAreaSize;
}

Configuration::Configuration(EEPROM_Rotate &eeprom) : eeprom(eeprom) {
	reservedAreaSize = getReservedAreaSize(eeprom);
	eeprom.begin(reservedAreaSize + sizeof(data));

#ifdef DEBUG_CONFIGURATION_PORT
	long eepromSize = Configuration::eeprom.reserved() * SPI_FLASH_SEC_SIZE;
	DEBUG_CONFIGURATION("[EEPROM] EEPROM size        : %ld bytes / %ld sectors\n", eepromSize, (eepromSize + SPI_FLASH_SEC_SIZE - 1) / SPI_FLASH_SEC_SIZE);
	DEBUG_CONFIGURATION("[EEPROM] Reserved sectors   : %u\n", eeprom.reserved());
	DEBUG_CONFIGURATION("[EEPROM] Sector pool size   : %u\n", eeprom.size());
	DEBUG_CONFIGURATION("[EEPROM] Sectors in use     : ");
	for (uint_fast8_t i = 0; i < eeprom.size(); ++i) {
		if (i > 0) {
			DEBUG_CONFIGURATION(", ");
		}
		DEBUG_CONFIGURATION("%d", eeprom.base() - i);
	}
	Serial.println();
	DEBUG_CONFIGURATION("[EEPROM] Current sector     : %u\n", eeprom.current());
	DEBUG_CONFIGURATION("[CONFIG] Store size         : %zu bytes\n", eeprom.length());
	DEBUG_CONFIGURATION("[CONFIG] Reserved area size : %u bytes\n", reservedAreaSize);
#endif

	size_t sizeofReadyData = eeprom.get(reservedAreaSize, sizeofReadyData);

	DEBUG_CONFIGURATION("[CONFIG] Data size          : %u bytes\n", sizeof(data));
	DEBUG_CONFIGURATION("[CONFIG] EEPROM data size   : %u bytes\n", sizeofReadyData);

	if (sizeofReadyData == sizeof(data)) {
		DEBUG_CONFIGURATION("[CONFIG] Read from sector %d\n", eeprom.current());
		memcpy(&data, eeprom.getConstDataPtr() + reservedAreaSize, sizeof(data));
	} else if (sizeofReadyData > sizeof(data)) {
		DEBUG_CONFIGURATION("[CONFIG] Reinitializing\n");
		data=getConfigurationDefault();
	} else if (sizeofReadyData < sizeof(data)) {
		DEBUG_CONFIGURATION("[CONFIG] Partially (%zu bytes of %zu) read from sector %d\n", sizeofReadyData, sizeof(data), eeprom.current());
		data=getConfigurationDefault();
		memcpy(&data, eeprom.getConstDataPtr() + reservedAreaSize, sizeofReadyData);
	}
}

bool Configuration::commit(EEPROM_Rotate &eeprom) {
	DEBUG_CONFIGURATION("[CONFIG] Commit to EEPROM\n");
	data.fix_size();
	eeprom.put(reservedAreaSize, data);
	return eeprom.commit();
}

const Configuration_data &Configuration::getData() const {
	return data;
}
