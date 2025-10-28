#include "logger.h"



Logger * Logger::logger;

VKAPI_ATTR VkBool32 VKAPI_CALL vkInit::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageSeverityFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	//if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	//    std::cerr << "[VULKAN WARNING] " << pCallbackData->pMessage << std::endl;
	//else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	//    std::cerr << "[VULKAN ERROR] " << pCallbackData->pMessage << std::endl;
	//else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
	//    std::cout << "[VULKAN INFO] " << pCallbackData->pMessage << std::endl;

	//// Returning false means �don�t abort the Vulkan call that triggered this message�
	//return VK_FALSE;

	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

vk::DebugUtilsMessengerEXT vkInit::make_debug_messenger(vk::Instance& instance, VkDebugUtilsMessengerEXT& messenger) {
	vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
		vk::DebugUtilsMessengerCreateFlagsEXT(),
		vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
		vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
		debugCallback,
		nullptr
	);
	auto createFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	createFunc(instance, createInfo, nullptr, &messenger);
	return messenger;
}


vk::Result vkInit::destroy_debug_messenger(vk::Instance& instance, VkDebugUtilsMessengerEXT& messenger) {
	auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	destroyFunc(instance, messenger, nullptr);
	return static_cast<vk::Result>(VK_SUCCESS);

}


void Logger::set_mode(bool mode) {
	enabled = mode;
}

Logger* Logger::get_logger() {
	if (!logger) {
		logger = new Logger();
	}

	return logger;
}

bool Logger::is_enabled() {
	return enabled;
}

void Logger::print(std::string message) {
	if (!enabled) return;
	std::cout << message << std::endl;
}

void Logger::report_version_number(uint32_t version) {
	if (!enabled) return;

	std::cout << "System can support Vulkan Variant: " << vk::apiVersionVariant(version)
		<< ", Major: " << vk::apiVersionMajor(version)
		<< ", Minor: " << vk::apiVersionMinor(version)
		<< ", Patch: " << vk::apiVersionPatch(version)
		<< std::endl;
}

void Logger::print_list(const char** list, uint32_t count) {
	if (!enabled) return;

	for (uint32_t i = 0; i < count; i++) {
		std::cout << "\t\"" << list[i] << "\"" << std::endl;
	}
}

void Logger::print_extensions(const std::vector<vk::ExtensionProperties>& extensions) {
	if (!enabled) return;

	for (vk::ExtensionProperties extension : extensions) {
		std::cout << "\t\"" << extension.extensionName << "\"" << std::endl;
	}
}

void Logger::print_layers(const std::vector<vk::LayerProperties>& layers) {
	if (!enabled) return;

	for (vk::LayerProperties layer : layers) {
		std::cout << "\t\"" << layer.layerName << "\"" << std::endl;
	}
}

void Logger::logDevice(const vk::PhysicalDevice& physicalDevice) {
	if (!enabled) return;

	vk::PhysicalDeviceProperties2 physicalDeviceProperties = physicalDevice.getProperties2();

	std::cout << "Device Name: " << physicalDeviceProperties.properties.deviceName << std::endl;

	std::cout << "Device type: ";
	switch (physicalDeviceProperties.properties.deviceType) {
	case (vk::PhysicalDeviceType::eCpu):
		std::cout << "CPU" << std::endl;
		break;
	
	case (vk::PhysicalDeviceType::eDiscreteGpu):
		std::cout << "Discrete GPU" << std::endl;
		break;
	
	case (vk::PhysicalDeviceType::eIntegratedGpu):
		std::cout << "Integrated GPU" << std::endl;
		break;

	case (vk::PhysicalDeviceType::eVirtualGpu):
		std::cout << "Virtual GPU" << std::endl;
		break;
	
	default:
		std::cout << "Other" << std::endl;

	}
	
}