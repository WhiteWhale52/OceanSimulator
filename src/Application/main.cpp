#include "Engine.h"
#include <Logging/Logger.h>
#include <Vulkan/VulkanContext.h>
#include <Vulkan/VulkanInit.h>
#include "AppConfig.h"

int main() {
	
	Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
	logger->set_mode(true);

	AppConfig appConfig{};

	Core::Vulkan::VulkanContext context{};

	Core::Vulkan::CreateInstance(context);
	Core::Vulkan::ChoosePhysicalDevice(context);
	Core::Vulkan::CreateDeviceAndQueues(context);
	Core::Vulkan::CreateCommandPools(context);

	Engine* graphicsEngine = new Engine();
	delete graphicsEngine;

	return 0;
}
