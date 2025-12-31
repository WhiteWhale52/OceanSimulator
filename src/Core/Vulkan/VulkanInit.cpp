#include "VulkanInit.h"
#include "Logging/Logger.h"

namespace Core::Vulkan {


	void CreateInstance(VulkanContext& context)
	{
		Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();


		logger->print("Making an instance");

		/*
		* An instance stores all per-application state info, it is a vulkan handle
		* (An opaque integer or pointer value used to refer to a Vulkan object)
		* side note: in the vulkan.hpp binding it's a wrapper class around a handle
		*
		* from vulkan_core.h:
		* VK_DEFINE_HANDLE(VkInstance)
		*
		* from vulkan_handles.hpp:
		* class Instance {
		* ...
		* }
		*/


		uint32_t version = vk::enumerateInstanceVersion();

		logger->report_version_number(version);

		version &= ~(0xFFFU);

		vk::ApplicationInfo appInfo = vk::ApplicationInfo(
			"Ocean Simulator",
			version,
			"Doing it the right way",
			version,
			version
		);


		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		// This is where we add any new extensions we want to check
		// extensions were the extensions required by glfw
		// Then we add any more extensions for other purposes 
#if DEBUG_VULKAN
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif



		logger->print("extensions to be requested");
		for (const char* extensionName : extensions) {
			std::cout << "\t\"" << extensionName << "\"\n";
		}
		logger->print_list(&extensions[0], extensions.size());


		std::vector<const char*> layers;

#if DEBUG_VULKAN
			layers.push_back("VK_LAYER_KHRONOS_validation");
#endif

		if (!InstanceSupported(extensions, layers)) {
			context.instance = VK_NULL_HANDLE;
		}


		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&appInfo,
			static_cast<uint32_t>(layers.size()),
			layers.data(),
			static_cast<uint32_t>(extensions.size()),
			extensions.data()
		);

		try {
			context.instance = vk::createInstance(createInfo);
		}
		catch (vk::SystemError err) {
#if DEBUG_VULKAN	
			std::cout << "Failed to create the instance \n";
#endif
		}
	}
	void ChoosePhysicalDevice(VulkanContext& context)
	{

	}
	void CreateDeviceAndQueues(VulkanContext& context)
	{
	}
	void CreateCommandPools(VulkanContext& context)
	{
	}
	void Destroy(VulkanContext& context)
	{
	}




	bool InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers)
	{
		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
		std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#if DEBUG_VULKAN
		std::cout << "\nThe device supports the following extensions: \n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
			std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
		}
#endif 
		bool found;
		for (const char* extension : extensions) {
			found = false;
			for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
				if (strcmp(supportedExtension.extensionName, extension) == 0) {
					found = true;
					std::cout << "\nExtension \"" << extension << "\" is supported";
				}
			}
			if (!found) {
#if DEBUG_VULKAN		
					std::cout << "\nExtension \"" << extension << "\" is not supported";
#endif
					return false;
			}
		}

#if DEBUG_VULKAN
			std::cout << "\nThe device supports the following layers: \n";
			for (vk::LayerProperties supportedLayer : supportedLayers) {
				std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
			}
#endif
		for (const char* layer : layers) {
			found = false;
			for (vk::LayerProperties supportedLayer : supportedLayers) {
				if (strcmp(supportedLayer.layerName, layer) == 0) {
					found = true;
					std::cout << "Layer \"" << layer << "\" is supported\n";
				}
			}
			if (!found) {
#if DEBUG_VULKAN
					std::cout << "Layer \"" << layer << "\" is not supported\n";
#endif				
					return false;
			}
		}


		return true;
	}
}