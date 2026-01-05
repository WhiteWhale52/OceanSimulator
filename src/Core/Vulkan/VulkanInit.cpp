#include "VulkanInit.h"
#include "PhysicalDeviceSupport.h"

namespace Core::Vulkan {


	void CreateInstance(VulkanContext& context, const Core::Config::AppConfig& appConfig)
	{

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
			appConfig.appName,
			(appConfig.appVersion == 0) ? version : appConfig.appVersion,
			appConfig.engineName,
			(appConfig.engineVersion == 0) ? version : appConfig.engineVersion,
			version
		);


		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

		// This is where we add any new extensions we want to check
		// extensions were the extensions required by glfw
		// Then we add any more extensions for other purposes 
#if DEBUG_VULKAN
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif



		logger->print("extensions to be requested");
		for (const char* extensionName : extensions) {
			logger->print( "\t\"", extensionName ,"\"\n");
		}
		//logger->print_list(&extensions[0], extensions.size());


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


	/// <summary>
	/// Chooses a Physical Device
	/// Sets up the context's queue family indices
	/// </summary>
	/// <param name="context">Global Vulkan Context</param>
	void ChoosePhysicalDevice(VulkanContext& context)
	{
		logger->print("Choosing physical device...");
		std::vector<vk::PhysicalDevice> physicalDevices = context.instance.enumeratePhysicalDevices();

		for (vk::PhysicalDevice physicalDevice : physicalDevices) {
			logger->logDevice(physicalDevice);
			PhysicalDeviceRequirements reqs;
			reqs.requiredExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
			if (IsDeviceSuitable(physicalDevice, reqs)) {
				context.physicalDevice = physicalDevice;
				uint32_t queueFamilyCount = 0;
				std::vector<vk::QueueFamilyProperties> queueFamiliesProperties = physicalDevice.getQueueFamilyProperties();
				logger->print("\nPhysical Device supports", queueFamiliesProperties.size(), "Queue Families.");
				for (uint32_t i = 0; i < queueFamiliesProperties.size(); i++) {
#if DEBUG_VULKAN
					logger->print("Queue Family", i);
					logger->print("Has", queueFamiliesProperties[i].queueCount, "queues");
					logger->print("Supports graphics:", bool(queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eGraphics));
					logger->print("Supports compute:", bool(queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eCompute));
					logger->print("Supports sparse binding:", bool(queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eSparseBinding));
#endif
					bool choseGraphicsQFamily = false;
					bool choseComputeQFamily = false;
					bool choseTransferQFamily = false;
					bool chosesparseBindingQFamily = false;
					

					if (queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eGraphics && !choseGraphicsQFamily)
					{
						context.graphicsQueueFamily = i;
						logger->print("\nGraphics Queue Family Index:", i);
						choseGraphicsQFamily = true;
					}

					if (queueFamiliesProperties[i].queueFlags & vk::QueueFlagBits::eCompute && 
						choseGraphicsQFamily && !choseComputeQFamily) {
						context.computeQueueFamily = i;
						logger->print("\nCompute Queue Family Index:", i);
						choseComputeQFamily = true;
					}

				}

				return;
			}
		}

	}
	
	
	void CreateDeviceAndQueues(VulkanContext& context)
	{
		std::set<uint32_t> uniqueQueueFamilies = {
		   context.graphicsQueueFamily,
		   context.computeQueueFamily
		};

		float priority = 1.0f;
		std::vector<vk::DeviceQueueCreateInfo> deviceQueuesCI;

		for (uint32_t queueFamily : uniqueQueueFamilies) {

			vk::DeviceQueueCreateInfo queueCreateInfo;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &priority;
			deviceQueuesCI.emplace_back(queueCreateInfo);
		}

		vk::PhysicalDeviceFeatures phyDeviceFeatures = context.physicalDevice.getFeatures();
		phyDeviceFeatures.geometryShader = VK_TRUE;
		phyDeviceFeatures.samplerAnisotropy = VK_TRUE;
		phyDeviceFeatures.sparseBinding = VK_TRUE;
		phyDeviceFeatures.tessellationShader = VK_TRUE;


		std::vector<const char*> deviceExtensions;
		deviceExtensions.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		

		vk::DeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCI.size());
		deviceCreateInfo.pQueueCreateInfos = deviceQueuesCI.data();
		deviceCreateInfo.pEnabledFeatures = &phyDeviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
#if DEBUG_VULKAN
		std::vector<const char*> layers{
			"VK_LAYER_KHRONOS_validation",
		};
		deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		deviceCreateInfo.ppEnabledLayerNames = layers.data();
		Logging::Logger* logger = Logging::Logger::get_logger();
#endif
		context.logicalDevice = context.physicalDevice.createDevice(deviceCreateInfo);
		
		context.computeQueue = context.logicalDevice.getQueue(context.computeQueueFamily, 0);
		context.graphicsQueue = context.logicalDevice.getQueue(context.graphicsQueueFamily, 0);


#if DEBUG_VULKAN
		logger->print("Logical Device Creation Successful.");
#endif
	}
	
	
	void CreateCommandPools(VulkanContext& context)
	{
	}




	bool static InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers)
	{
		std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
		std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

#if DEBUG_VULKAN
		std::cout << "\nThe instance supports the following extensions: \n";
		for (const vk::ExtensionProperties& supportedExtension : supportedExtensions) {
			std::cout << "\t\"" << supportedExtension.extensionName << "\"\n";
		}
#endif 
		bool found;
		for (const char* extension : extensions) {
			found = false;
			for (const vk::ExtensionProperties& supportedExtension : supportedExtensions) {
				if (strcmp(supportedExtension.extensionName, extension) == 0) {
					found = true;
					std::cout << "\nInstance Extension \"" << extension << "\" is supported";
				}
			}
			if (!found) {
#if DEBUG_VULKAN		
					std::cout << "\nInstance Extension \"" << extension << "\" is not supported";
#endif
					return false;
			}
		}

#if DEBUG_VULKAN
			std::cout << "\nThe instance supports the following layers: \n";
			for (const vk::LayerProperties& supportedLayer : supportedLayers) {
				std::cout << "\t\"" << supportedLayer.layerName << "\"\n";
			}
#endif
		for (const char* layer : layers) {
			found = false;
			for (const vk::LayerProperties& supportedLayer : supportedLayers) {
				if (strcmp(supportedLayer.layerName, layer) == 0) {
					found = true;
					std::cout << "Instance Layer \"" << layer << "\" is supported\n";
				}
			}
			if (!found) {
#if DEBUG_VULKAN
					std::cout << "Instance Layer \"" << layer << "\" is not supported\n";
#endif				
					return false;
			}
		}


		return true;
	}


	
	
	void Destroy(VulkanContext& context)
	{
		if (context.logicalDevice) {
			context.logicalDevice.waitIdle();
		}

		if (context.computeCmdPool) {
			context.logicalDevice.destroyCommandPool(context.computeCmdPool);
			context.computeCmdPool = VK_NULL_HANDLE;
		}

		if (context.graphicsCmdPool) {
			context.logicalDevice.destroyCommandPool(context.graphicsCmdPool);
			context.graphicsCmdPool = VK_NULL_HANDLE;
		}

		if (context.logicalDevice) {
			context.logicalDevice.destroy();
			context.logicalDevice = VK_NULL_HANDLE;
		}


		if (context.instance) {
			context.instance.destroy();
			context.instance = VK_NULL_HANDLE;
		}

	}
}
	