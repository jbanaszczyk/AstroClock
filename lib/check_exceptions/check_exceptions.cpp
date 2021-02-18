#include <Arduino.h>
#include <exception>

#include "check_exceptions.h"

bool check_try_catch(bool verbose) {
	bool catched = false;
#ifdef CHECK_EXCEPTIONS
	if (verbose) {
		Serial.printf( PSTR("Checking exceptions\n");
	}
	try {
		throw std::runtime_error("exception thrown.");
	} catch (const std::exception &e) {
		catched = true;
		if (verbose) {
			Serial.printf(PSTR("Exception caught: %s\n"), e.what());
		}
	}
#endif
	return catched;
}
