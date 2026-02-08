#include "Engine.h"
#include <Logging/Logger.h>
#include <Vulkan/VulkanContext.h>
#include <Vulkan/VulkanInit.h>
#include <Config/appConfig.h>

int main() {
	
	Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();

	Core::Config::AppConfig appConfig{};
	appConfig.appName = "Ocean Waves";
	appConfig.engineName = "Engine Name";

	Core::Vulkan::VulkanContext context{};

	Core::Vulkan::CreateInstance(context, appConfig);
	Core::Vulkan::ChoosePhysicalDevice(context);
	Core::Vulkan::CreateDeviceAndQueues(context);
	Core::Vulkan::CreateCommandPools(context);

//	Engine::Engine* graphicsEngine = new Engine::Engine();
//	delete graphicsEngine;
	Core::Vulkan::Destroy(context);
	return 0;
}
