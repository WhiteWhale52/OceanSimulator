#include "logger.h"

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

    //// Returning false means “don’t abort the Vulkan call that triggered this message”
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