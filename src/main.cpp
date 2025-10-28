#include "engine.h"
#include "logger.h"

int main() {
	Logger* logger = Logger::get_logger();
	logger->set_mode(true);

	Engine* graphicsEngine = new Engine();
	delete graphicsEngine;

	return 0;
}
