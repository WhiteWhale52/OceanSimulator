#pragma once
#include <VulkanCore/Config/CommonHeaders.h>
#include <VulkanCore/Config/ResourcesConfigs.h>
#include <VulkanCore/VulkanContext.h>

namespace Renderer::Vulkan {

	struct Buffer {
		void CreateVertexBuffer(const Core::Vulkan::VulkanContext& context, const void* data, size_t size);
		vk::Buffer handle = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		size_t size = 0;

		void Create(const Core::Vulkan::VulkanContext& context);
		void Destroy(const Core::Vulkan::VulkanContext& context);
	};


}
