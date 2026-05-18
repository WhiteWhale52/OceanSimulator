#pragma once
#include <VulkanCore/Config/CommonHeaders.h>
#include <VulkanCore/Config/ResourcesConfigs.h>
#include <VulkanCore/VulkanContext.h>

namespace Renderer::Vulkan {
	struct RenderPass {
		vk::RenderPass handle = VK_NULL_HANDLE;
		void Create(const Core::Vulkan::VulkanContext& context, vk::Format colorFormat, vk::Format depthFormat);
		void Destroy(const Core::Vulkan::VulkanContext& context);
	};

	struct FrameBuffer {
		vk::Framebuffer handle = VK_NULL_HANDLE;
		void Create(const Core::Vulkan::VulkanContext& context, RenderPass& renderPass, vk::ImageView colorView, vk::ImageView depthView, uint32_t width, uint32_t height);
		void Destroy(const Core::Vulkan::VulkanContext& context);
	};
}