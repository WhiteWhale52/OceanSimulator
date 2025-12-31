#pragma once
#include "config.h"

namespace Core::Vulkan {
	struct VulkanContext {
		vk::Instance instance = VK_NULL_HANDLE;

		vk::PhysicalDevice physicalDevice = VK_NULL_HANDLE;
		vk::Device logicalDevice = VK_NULL_HANDLE;

		uint32_t graphicsQueueFamily = UINT32_MAX;
		uint32_t computeQueueFamily = UINT32_MAX;

		vk::Queue graphicsQueue = VK_NULL_HANDLE;
		vk::Queue computeQueue = VK_NULL_HANDLE;

		vk::CommandPool graphicsCmdPool = VK_NULL_HANDLE;
		vk::CommandPool computeCmdPool = VK_NULL_HANDLE;
	};
	VulkanContext context;
}