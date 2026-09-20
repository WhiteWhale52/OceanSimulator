#pragma once
#include <VulkanCore/Config/CommonHeaders.h>
#include <VulkanCore/Config/ResourcesConfigs.h>
#include <VulkanCore/VulkanContext.h>

namespace TheRenderer::Vulkan {
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

	class SwapchainResources {
	public:
		void Create(const Core::Vulkan::VulkanContext& context, vk::Format colourFormat, vk::Format depthFormat,
			const std::vector<vk::ImageView>& colouriews, vk::ImageView depthView, uint32_t width, uint32_t height);

		void Destroy(const Core::Vulkan::VulkanContext& context);

		RenderPass renderPass;
		std::vector<FrameBuffer> framebuffers;
	};
}