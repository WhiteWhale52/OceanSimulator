#pragma once

#include <vulkan/vulkan.hpp>
#include <VulkanCore/VulkanContext.h> 
#include "SwapchainResources.h"

namespace TheRenderer::Vulkan {
	class CommandBuffers {
	public:
		CommandBuffers(Core::Vulkan::VulkanContext& context);
	
		vk::CommandBuffer AllocateGraphicsCmdBuffer();
		vk::CommandBuffer AllocateComputeCmdBuffer();
		void RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex, const SwapchainResources& swapchainResources,
			vk::Extent2D swapchainExtent, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, uint32_t vertexCount);

	private:
		Core::Vulkan::VulkanContext& m_context;
	};
}
