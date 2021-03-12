#include <Arduino.h>
#include <EEPROM_Rotate.h>
#include <spi_flash.h>

EEPROM_Rotate EEPROMr;

extern "C" uint32_t _SPIFFS_start;
extern "C" uint32_t _SPIFFS_end;

static unsigned int info_bytes2sectors(size_t size) {
	return (int) (size + SPI_FLASH_SEC_SIZE - 1) / SPI_FLASH_SEC_SIZE;
}

static unsigned long info_ota_space() {
	return (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
}

static unsigned long info_filesystem_space() {
	return ((uint32_t) &_SPIFFS_end - (uint32_t) &_SPIFFS_start);
}

static unsigned long info_eeprom_space() {
	return EEPROMr.reserved() * SPI_FLASH_SEC_SIZE;
}

static void info_print_memory_layout_line(const char *name, unsigned long bytes, bool reset) {
	static unsigned long index = 0;
	if (reset)
		index = 0;
	if (0 == bytes)
		return;
	unsigned int _sectors = info_bytes2sectors(bytes);
	Serial.printf("[INIT] %-20s: %8lu bytes / %4d sectors (%4ld to %4ld)\n", name, bytes, _sectors, index, index + _sectors - 1);
	index += _sectors;
}

static void info_print_memory_layout_line(const char *name, unsigned long bytes) {
	info_print_memory_layout_line(name, bytes, false);
}

void get_memory_info() {
	info_print_memory_layout_line("Flash size (CHIP)", ESP.getFlashChipRealSize(), true);
	info_print_memory_layout_line("Flash size (SDK)", ESP.getFlashChipSize(), true);
	info_print_memory_layout_line("Reserved", 1 * SPI_FLASH_SEC_SIZE, true);
	info_print_memory_layout_line("Firmware size", ESP.getSketchSize());
	info_print_memory_layout_line("Max OTA size", info_ota_space());
	info_print_memory_layout_line("SPIFFS size", info_filesystem_space());
	info_print_memory_layout_line("EEPROM size", info_eeprom_space());
	info_print_memory_layout_line("Reserved", 4 * SPI_FLASH_SEC_SIZE);

	Serial.println();

	// EEPROM Initialization ---------------------------------------------------

	// Not needed if we have already reserved several sectors in the memory layout
	EEPROMr.size(2);

	// Load data
	EEPROMr.begin(4096);

	// EEPROM Info -------------------------------------------------------------
	Serial.println();
	Serial.printf("[EEPROM] Reserved sectors : %u\n", EEPROMr.reserved());
	Serial.printf("[EEPROM] Sector pool size : %u\n", EEPROMr.size());
	Serial.printf("[EEPROM] Sectors in use   : ");
	for (uint_fast8_t i = 0; i < EEPROMr.size(); i++) {
		if (i > 0)
			Serial.print(", ");
		Serial.print(EEPROMr.base() - i);
	}
	Serial.println();
	Serial.printf("[EEPROM] Current sector   : %u\n", EEPROMr.current());
}

void setup() {
	Serial.begin(115200);
	delay(2000);
	while (!Serial) {
	}
	Serial.println();
	Serial.println();

	get_memory_info();
}

void loop() {}

// Original map
//[INIT] Flash size (CHIP)   :  4194304 bytes / 1024 sectors (   0 to 1023)
//[INIT] Flash size (SDK)    :  4194304 bytes / 1024 sectors (   0 to 1023)
//[INIT] Reserved            :     4096 bytes /    1 sectors (   0 to    0)
//[INIT] Firmware size       :   268368 bytes /   66 sectors (   1 to   66)
//[INIT] Max OTA size        :  2871296 bytes /  701 sectors (  67 to  767)
//[INIT] SPIFFS size         :  1024000 bytes /  250 sectors ( 768 to 1017)
//[INIT] EEPROM size         :     8192 bytes /    2 sectors (1018 to 1019)
//[INIT] Reserved            :    16384 bytes /    4 sectors (1020 to 1023)
