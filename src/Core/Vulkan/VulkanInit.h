#pragma once

#include "VulkanContext.h"
#include "Config/AppConfig.h"
#include "Logging/Logger.h"


namespace Core::Vulkan {
	void CreateInstance(VulkanContext& context, const Core::Config::AppConfig& appConfig);
	bool InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers);


	void ChoosePhysicalDevice(VulkanContext& context);

	void CreateDeviceAndQueues(VulkanContext& context);

	void CreateCommandPools(VulkanContext& context);
	//void AllocateCmdBufferCompute(const VulkanContext& context);
	//void AllocateCmdBufferGraphics(const VulkanContext& context);

	void CreateSurface(VulkanContext& context, GLFWwindow* window);

	void Destroy(VulkanContext& context);
	
	extern PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT_Func = nullptr;
}
