#pragma once

#include "VulkanContext.h"

namespace Core::Vulkan {
	void CreateInstance(VulkanContext& context);
	bool InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers);


	void ChoosePhysicalDevice(VulkanContext& context);

	void CreateDeviceAndQueues(VulkanContext& context);

	void CreateCommandPools(VulkanContext& context);

	void Destroy(VulkanContext& context);

}