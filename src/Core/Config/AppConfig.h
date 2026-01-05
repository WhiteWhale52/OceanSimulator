#pragma once
#include <iostream>

namespace Core::Config {

	struct AppConfig {
		const char* appName;
		uint32_t appVersion = 0;

		const char* engineName;
		uint32_t engineVersion = 0;
	};
}

