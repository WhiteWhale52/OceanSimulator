#pragma once

#include "VulkanContext.h"

namespace Core::Vulkan {
	void CreateInstance(VulkanContext&);
	bool InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers);


	void ChoosePhysicalDevice(VulkanContext&);

	void CreateDeviceAndQueues(VulkanContext&);

	void CreateCommandPools(VulkanContext&);

	void Destroy(VulkanContext&);
}