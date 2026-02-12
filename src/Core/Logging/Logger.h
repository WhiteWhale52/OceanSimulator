#pragma once

#include "../Vulkan/config.h"

namespace Core::Logging {
	
	/*
	* Debug call back:
	*
	*	typedef enum VkDebugUtilsMessageSeverityFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageSeverityFlagBitsEXT;

	*	typedef enum VkDebugUtilsMessageTypeFlagBitsEXT {
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = 0x00000001,
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = 0x00000002,
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = 0x00000004,
			VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		} VkDebugUtilsMessageTypeFlagBitsEXT;

	*	typedef struct VkDebugUtilsMessengerCallbackDataEXT {
			VkStructureType                              sType;
			const void*                                  pNext;
			VkDebugUtilsMessengerCallbackDataFlagsEXT    flags;
			const char*                                  pMessageIdName;
			int32_t                                      messageIdNumber;
			const char*                                  pMessage;
			uint32_t                                     queueLabelCount;
			const VkDebugUtilsLabelEXT*                  pQueueLabels;
			uint32_t                                     cmdBufLabelCount;
			const VkDebugUtilsLabelEXT*                  pCmdBufLabels;
			uint32_t                                     objectCount;
			const VkDebugUtilsObjectNameInfoEXT*         pObjects;
		} VkDebugUtilsMessengerCallbackDataEXT;

	*/


	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);


	vk::DebugUtilsMessengerEXT make_debug_messenger(vk::Instance& instance);
	
	
	void destroy_debug_messenger(vk::Instance& instance,  vk::DebugUtilsMessengerEXT& messenger);


	
	class Logger {
	public:
		static Logger* logger;

		static Logger* get_logger();


		void print();
		
		
		template<typename T, typename... Args>
		void print(T first, Args... args) {
			std::cout << first;
			if constexpr (sizeof...(args) > 0) {
				std::cout << " ";
				print(args...);
			}
			else {
				std::cout << std::endl;
			}
		}

		void report_version_number(uint32_t version);

		void print_extensions(const std::vector<vk::ExtensionProperties>& extensions);

		void print_layers(const std::vector<vk::LayerProperties>& layers);

		void logDevice(const vk::PhysicalDevice& device);

		void print_list(const char** list, uint32_t const);


	private:

		bool enabled;
	};
	
}

