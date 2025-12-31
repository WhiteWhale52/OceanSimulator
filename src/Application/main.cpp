#include "Engine.h"
#include <Logging/Logger.h>

int main() {
	
	Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
	logger->set_mode(true);

	Engine* graphicsEngine = new Engine();
	delete graphicsEngine;

	return 0;
}
