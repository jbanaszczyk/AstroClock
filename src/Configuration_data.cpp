#include "Configuration_data.h"

Configuration_data *getConfigurationDefault() {
	static Configuration_data configurationDefault(10, 20);
	return &configurationDefault;
}
