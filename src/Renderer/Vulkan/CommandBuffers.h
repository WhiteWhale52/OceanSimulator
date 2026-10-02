#pragma once

#include <vulkan/vulkan.hpp>
#include <VulkanCore/VulkanContext.h> 
#include "Swapchain.h"

namespace TheRenderer::Vulkan {
	class CommandBuffers {
	public:
		CommandBuffers(Core::Vulkan::VulkanContext& context);
	
		vk::CommandBuffer AllocateGraphicsCmdBuffer();
		vk::CommandBuffer AllocateComputeCmdBuffer();
		void RecordCommandBuffer(vk::CommandBuffer commandBuffer, vk::RenderPass renderPass, vk::Framebuffer framebuffer, vk::Extent2D extent,
			vk::Pipeline graphicsPipeline,vk::PipelineLayout pipelineLayout, vk::DescriptorSet descriptorSet, vk::Buffer vertexBuffer, uint32_t vertexCount, 
			vk::Buffer indexBuffer, uint32_t indexCount);

	private:
		Core::Vulkan::VulkanContext& m_context;
	};
}
