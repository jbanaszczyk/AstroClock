#pragma once

#include <Arduino.h>

class Configuration_data {
private:
	size_t size;
	int someValue;
	int anotherValue;
public:
	Configuration_data() = default;

	void fix_size() {
		size = sizeof(Configuration_data);
	}

	int getSomeValue() const {
		return someValue;
	}

	void setSomeValue(int someValue) {
		this->someValue = someValue;
	}

	int getAnotherValue() const {
		return anotherValue;
	}

	void setAnotherValue(int anotherValue) {
		this->anotherValue = anotherValue;
	}

public:
	Configuration_data(int someValue, int anotherValue) :
			size(sizeof(Configuration_data)),
			someValue(someValue),
			anotherValue(anotherValue) {}
};

Configuration_data *getConfigurationDefault();