#include <Arduino.h>
#include <exception>

#include "check_exceptions.h"

void check_try_catch(boolean verbose) {
	try {
		throw std::runtime_error("exception thrown.");
	} catch (const std::exception &e) {
		if (verbose) {
			Serial.print("exception caught: ");
			Serial.println(e.what());
		}
	}
}
