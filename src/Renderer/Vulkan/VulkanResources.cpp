#include "VulkanResources.h"


namespace Renderer::Vulkan {
	void Buffer::CreateVertexBuffer(const Core::Vulkan::VulkanContext& context, const void* data, size_t size) {
		this->size = size;

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;


		VmaAllocationCreateInfo allocInfo;
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
}