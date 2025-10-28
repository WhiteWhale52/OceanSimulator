#include "instance.h"
#include "logger.h"

/// <summary>
/// Checks if the extensions and layers requested are supported
/// This functions should only be called once
/// </summary>
/// <param name="extensions">A list of extension names of the extensions that we request the instance to have</param>
/// <param name="layers">A list of layers names of the layers that we request the instance to have</param>
/// <param name="debug"></param>
/// <returns></returns>
bool vkInit::supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug)
{
	std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
	std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

	if (debug) {
		std::cout << "\nThe device supports the following extensions: \n";
		for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
			std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
		}
	}

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
			if (debug) {
				std::cout << "\nExtension \"" << extension << "\" is not supported";
			}
			return false;
		}
	}

	if (debug) {
		std::cout << "\nThe device supports the following layers: \n";
		for (vk::LayerProperties supportedLayer : supportedLayers) {
			std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
		}
	}

	for (const char* layer : layers) {
		found = false;
		for (vk::LayerProperties supportedLayer : supportedLayers) {
			if (strcmp(supportedLayer.layerName, layer) == 0) {
				found = true;
				std::cout << "Layer \"" << layer << "\" is supported\n";
			}
		}
		if (!found) {
			if (debug) {
				std::cout << "Layer \"" << layer << "\" is not supported\n";
			}
			return false;
		}
	}


	return true;
}



vk::Instance vkInit::make_instance(bool debug, const char* applicationName) {

	Logger* logger = Logger::get_logger();


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
		applicationName,
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
	if (debug) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}



	if (debug) {
		std::cout << "extensions to be requested \n";
		for (const char* extensionName : extensions) {
			std::cout << "\t\"" << extensionName << "\"\n";
		}
	}


	std::vector<const char*> layers;

	if (debug) {
		layers.push_back("VK_LAYER_KHRONOS_validation");
	}

	if (!supported(extensions, layers, debug)) {
		return nullptr;
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
		return vk::createInstance(createInfo);

	}
	catch (vk::SystemError err) {
		if (debug) {
			std::cout << "Failed to create the instance \n";
		}
		return nullptr;
	}
}

