#pragma once

#include <vulkan/vulkan.hpp>
#include <VulkanCore/VulkanContext.h> 

namespace Renderer::Vulkan {
	class CommandBuffers {
	public:
		CommandBuffers(Core::Vulkan::VulkanContext& context);
	
		vk::CommandBuffer AllocateGraphicsCmdBuffer();
		vk::CommandBuffer AllocateComputeCmdBuffer();
		void RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex, vk::RenderPass renderPass, 
			const std::vector<vk::Framebuffer>& frameBuffers, vk::Extent2D swapchainExtent, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer,
			uint32_t vertexCount);

	private:
		Core::Vulkan::VulkanContext& m_context;
	};
}
