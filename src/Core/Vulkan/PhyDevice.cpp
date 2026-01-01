#include "PhyDevice.h"
#include "../Logging/Logger.h"


bool supports(
	const vk::PhysicalDevice& physicalDevice,
	const char** ppRequestedExtensions,
	const uint32_t requestedExtensionCount) {
	
	Logger* logger = Logger::get_logger();
	logger -> print("The requested Physical Device Extensions");
	logger -> print_list(ppRequestedExtensions, requestedExtensionCount);

	std::vector<vk::ExtensionProperties> extensions = physicalDevice.enumerateDeviceExtensionProperties();
	logger->print("Physical Device Supported Extensions: ");
	logger->print_extensions(extensions);

	for (uint32_t i = 0; i < requestedExtensionCount; ++i) {
		bool supported = false;

		for (vk::ExtensionProperties& extension : extensions) {
			char* name = extension.extensionName;
			if (strcmp(name, ppRequestedExtensions[i]) == 0) {
				supported = true;
				break;
			}
		}
		if (!supported) {
			return false;
		}
	}
	return true;
}

bool is_suitable(const vk::PhysicalDevice& physicalDevice) {
	Logger* logger = Logger::get_logger();

	logger->print("Checking if device is suitable");

	const char* ppRequestedExtension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	if (supports(physicalDevice, &ppRequestedExtension, 1)) {
		logger->print("Device can support the requested extensions!");
	}
	else {
		logger->print("Device can't support the requested extensions!");
		return false;
	}
	return true;
}


vk::PhysicalDevice choose_physical_device(const vk::Instance& instance) {
	Logger* logger = Logger::get_logger();
	logger->print("Choosing physical device...");
	std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();

	for (vk::PhysicalDevice physicalDevice : physicalDevices) {
		logger->logDevice(physicalDevice);
		if (is_suitable(physicalDevice)) return physicalDevice;
	}
	return nullptr;

}