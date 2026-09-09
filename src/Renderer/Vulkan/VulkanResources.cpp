#include "VulkanResources.h"


namespace Renderer::Vulkan {
	void Buffer::CreateVertexBuffer(const Core::Vulkan::VulkanContext& context, const void* data, size_t size) {
		this->size = size;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


		VmaAllocationCreateInfo allocInfo{};
		allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
		allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

		VkBuffer rawBuffer = VK_NULL_HANDLE;

		VmaAllocationInfo outInfo{};
		if (vmaCreateBuffer(context.vmaAllocator, &bufferInfo, &allocInfo,
			&rawBuffer, &allocation, &outInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create vertex buffer");
		}

		handle = vk::Buffer(rawBuffer);

		void* mapped = nullptr;
		vmaMapMemory(context.vmaAllocator, allocation, &mapped);
		std::memcpy(mapped, data, size);
		vmaUnmapMemory(context.vmaAllocator, allocation);
	}

	void Buffer::Destroy(const Core::Vulkan::VulkanContext& context) {
		if (context.vmaAllocator && handle) {
			vmaDestroyBuffer(context.vmaAllocator, static_cast<VkBuffer>(handle), allocation);
		}
	}


	void Image::Create2D(const Core::Vulkan::VulkanContext& context, uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usageFlags, VkImageAspectFlags aspectFlags, VmaMemoryUsage memoryUsage)
	{
		this->height = height;
		this->width = width;
		this->aspectFlags = aspectFlags;
		this->format = format;

		VkImageCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.imageType = VK_IMAGE_TYPE_2D;
		createInfo.extent = { width, height, 1 };
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.format = format;
		createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		createInfo.usage = usageFlags;
		createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocationCreateInfo{};
		allocationCreateInfo.usage = memoryUsage;

		VkImage rawImage = VK_NULL_HANDLE;
		if (vmaCreateImage(context.vmaAllocator, &createInfo, &allocationCreateInfo, &rawImage, &allocation, nullptr) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image using VMA in VulkanResources.cpp");
		}
		handle = vk::Image(rawImage);

		vk::ImageViewCreateInfo imageViewCreateInfo;
		imageViewCreateInfo.image = rawImage;
		imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
		imageViewCreateInfo.format = vk::Format(format);
		imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlags(aspectFlags);
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		vk::ImageView rawView = VK_NULL_HANDLE;
		if (context.logicalDevice.createImageView(&imageViewCreateInfo, nullptr, &rawView) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create an image view in VulkanResources.cpp");
		}

		view = vk::ImageView(rawView);
	}

	void Image::TransitionLayout(vk::CommandBuffer commandBuffer, vk::ImageLayout oldLayout, vk::ImageLayout newLayout, vk::PipelineStageFlags srcStage, vk::PipelineStageFlags dstStage, vk::AccessFlags srcAccess, vk::AccessFlags dstAccess)
	{
		vk::ImageMemoryBarrier memoryBarrier;
		memoryBarrier.oldLayout = oldLayout;
		memoryBarrier.newLayout = newLayout;
		memoryBarrier.srcQueueFamilyIndex = vk::QueueFamilyIgnored;
		memoryBarrier.dstQueueFamilyIndex = vk::QueueFamilyIgnored;
		memoryBarrier.image = handle;
		memoryBarrier.subresourceRange.aspectMask = vk::ImageAspectFlags(aspectFlags);
		memoryBarrier.subresourceRange.baseMipLevel = 0;
		memoryBarrier.subresourceRange.baseArrayLayer = 0;
		memoryBarrier.subresourceRange.levelCount = 1;
		memoryBarrier.subresourceRange.layerCount = 1;
		memoryBarrier.srcAccessMask = srcAccess;
		memoryBarrier.dstAccessMask = dstAccess;

		commandBuffer.pipelineBarrier(srcStage, dstStage, {}, 0, nullptr, 0, nullptr, 1, &memoryBarrier);

	}

	void Image::Destroy(const Core::Vulkan::VulkanContext& context)
	{
		if (context.logicalDevice && view) {
			context.logicalDevice.destroyImageView(view);
		}
		if (context.vmaAllocator && handle) {
			vmaDestroyImage(context.vmaAllocator, static_cast<VkImage>(handle), allocation);
		}
	}
}