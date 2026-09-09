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

	struct Image {
		void Create2D(const Core::Vulkan::VulkanContext& context, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectFlags, VmaMemoryUsage memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY);

		void TransitionLayout(vk::CommandBuffer commandBuffer, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::PipelineStageFlags srcStage, vk::PipelineStageFlags dstStage, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess);

		void Destroy(const Core::Vulkan::VulkanContext& context);

		vk::Image handle = VK_NULL_HANDLE;
		vk::ImageView view = VK_NULL_HANDLE;
		VmaAllocation allocation = VK_NULL_HANDLE;
		
		uint32_t width = 0;
		uint32_t height = 0;

		VkFormat format = VK_FORMAT_UNDEFINED;
		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

	};


}
