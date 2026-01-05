#pragma once
#include "config.h"
#include <Logging/Logger.h>

namespace Core::Vulkan {

	struct PhysicalDeviceRequirements {
		std::vector<const char*> requiredExtensions;
		bool requireGraphics = true;
		bool requireCompute = true;
		bool requirePresent = false;
		vk::PhysicalDeviceType requiredDeviceType = vk::PhysicalDeviceType::eDiscreteGpu;
		PhysicalDeviceRequirements() = default;
	};

	bool IsDeviceSuitable(const vk::PhysicalDevice& physicalDevice, const PhysicalDeviceRequirements& reqs) 
	{
		Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
		vk::PhysicalDeviceProperties physicalDeviceProperties = physicalDevice.getProperties2().properties;
		if (physicalDeviceProperties.deviceType != reqs.requiredDeviceType) {
			return false;
		}
		logger->print(physicalDeviceProperties.deviceName);
		logger->print("The requested Physical Device Extensions");
		for (const char* requestedExtension : reqs.requiredExtensions) {
			logger->print("\t\"",requestedExtension, "\"");
		}
		std::vector<vk::ExtensionProperties> extensions = physicalDevice.enumerateDeviceExtensionProperties();
		logger->print("Physical Device Supported Extensions: ");
		
		for (const char* requestedExtension : reqs.requiredExtensions) {
			bool supported = false;

			for (vk::ExtensionProperties& extension : extensions) {
				char* name = extension.extensionName;
				if (strcmp(name, requestedExtension) == 0) {
					supported = true;
					logger->print("\tPhysical Device supports", requestedExtension);
					break;
				}
			}
			if (!supported) {
				logger->print("\tPhysical Device doesn't support the requested extensions");
				return false;
			}
		}

		
	}

}