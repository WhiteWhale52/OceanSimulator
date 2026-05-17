#pragma once

#include "VulkanContext.h"
#include "Config/AppConfig.h"
#include "Logging/Logger.h"
#include <GLFW/glfw3.h>


namespace Core::Vulkan {
	void CreateInstance(VulkanContext& context, const Core::Config::AppConfig& appConfig);
	bool InstanceSupported(std::vector<const char*>& extensions, std::vector<const char*>& layers);
	void CreateGLFWWindow(VulkanContext& context);

	void ChoosePhysicalDevice(VulkanContext& context);

	void CreateDeviceAndQueues(VulkanContext& context);

	void CreateCommandPools(VulkanContext& context);
	//void AllocateCmdBufferCompute(const VulkanContext& context);
	//void AllocateCmdBufferGraphics(const VulkanContext& context);

	void CreateSurface(VulkanContext& context, GLFWwindow* window);

	void VMASetUp(VulkanContext& context);

	void Destroy(VulkanContext& context);

	int width{ 640 };
	int height{ 480 };
	GLFWwindow* window{ nullptr };
#if DEBUG_VULKAN
	Logging::Logger* logger = Core::Logging::Logger::get_logger();
#endif

	
}
