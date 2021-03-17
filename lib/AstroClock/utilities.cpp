#include "utilities.h"

#include <Arduino.h>
#include <user_interface.h>

uint32_t initFreeHeapSize = 65536;

const char *getLastRestartReason_P() {
	static const char *RESTART_REASONS[] = {
			PSTR("Power reboot"),
			PSTR("Hardware WDT reset"),
			PSTR("Fatal exception"),
			PSTR("Software watchdog reset"),
			PSTR("Software reset"),
			PSTR("Deep-sleep"),
			PSTR("Hardware reset"),
			PSTR("Unknown"),
	};
	return RESTART_REASONS[min(
			system_get_rst_info()->reason,
			static_cast<uint32>(ARRAYSIZE(RESTART_REASONS) - 1))];
}

char *strcpy_s(char *destination, const char *source, size_t max_len) {
	max_len = min(strlen(source), max_len - 1);
	strncpy(destination, source, max_len);
	destination[max_len] = '\0';
	return destination;
}

char *strcpy_P_s(char *destination, const char *source, size_t max_len) {
	max_len = min(strlen_P(source), max_len - 1);
	strncpy_P(destination, source, max_len);
	destination[max_len] = '\0';
	return destination;
}

void printTm(const char* what, const tm* tm) {
	Serial.print(what);
	PTM(isdst); PTM(yday); PTM(wday);
	PTM(year);  PTM(mon);  PTM(mday);
	PTM(hour);  PTM(min);  PTM(sec);
}
