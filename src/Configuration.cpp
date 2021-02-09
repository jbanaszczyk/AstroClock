#include "Configuration.h"

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

#ifdef DEBUG_CONFIGURATION
	long eepromSize = Configuration::eeprom.reserved() * SPI_FLASH_SEC_SIZE;
	Serial.printf("[EEPROM] EEPROM size        : %ld bytes / %ld sectors\n", eepromSize, (eepromSize + SPI_FLASH_SEC_SIZE - 1) / SPI_FLASH_SEC_SIZE);
	Serial.printf("[EEPROM] Reserved sectors   : %u\n", eeprom.reserved());
	Serial.printf("[EEPROM] Sector pool size   : %u\n", eeprom.size());
	Serial.printf("[EEPROM] Sectors in use     : ");
	for (uint_fast8_t i = 0; i < eeprom.size(); ++i) {
		if (i > 0) {
			Serial.print(", ");
		}
		Serial.print(eeprom.base() - i);
	}
	Serial.println();
	Serial.printf("[EEPROM] Current sector     : %u\n", eeprom.current());
	Serial.printf("[CONFIG] Store size         : %zu bytes\n", eeprom.length());
	Serial.printf("[CONFIG] Reserved area size : %u bytes\n", reservedAreaSize);
#endif

	size_t sizeofReadyData = eeprom.get(reservedAreaSize, sizeofReadyData);

	Serial.printf("[CONFIG] Data size          : %u bytes\n", sizeof(data));
	Serial.printf("[CONFIG] EEPROM data size   : %u bytes\n", sizeofReadyData);

	if (sizeofReadyData == sizeof(data)) {
		Serial.printf("[CONFIG] Read from sector %d\n", eeprom.current());
		memcpy(&data, eeprom.getConstDataPtr() + reservedAreaSize, sizeof(data));
	} else if (sizeofReadyData > sizeof(data)) {
		Serial.printf("[CONFIG] reinitializing\n");
		memcpy(&data, getConfigurationDefault(), sizeof(data));
	} else if (sizeofReadyData < sizeof(data)) {
		Serial.printf("[CONFIG] Partially (%zu bytes of %zu) read from sector %d\n", sizeofReadyData, sizeof(data), eeprom.current());
		memcpy(&data, getConfigurationDefault(), sizeof(data));
		memcpy(&data, eeprom.getConstDataPtr() + reservedAreaSize, sizeofReadyData);
//		commit(eeprom);
	}

	Serial.println("___________");
//	Serial.println(data.getSomeValue());
//	Serial.println(data.getAnotherValue());
}

bool Configuration::commit(EEPROM_Rotate &eeprom) {
	data.fix_size();
	eeprom.put(reservedAreaSize, data);
	return eeprom.commit();
}

const Configuration_data &Configuration::getData() const {
	return data;
}
