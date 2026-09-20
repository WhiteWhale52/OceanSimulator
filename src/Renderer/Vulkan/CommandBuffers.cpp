#include "CommandBuffers.h"


namespace TheRenderer::Vulkan {

	CommandBuffers::CommandBuffers(Core::Vulkan::VulkanContext& context) : m_context(context)
	{
	}

	vk::CommandBuffer CommandBuffers::AllocateGraphicsCmdBuffer()
	{
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandBufferCount = 1;
		allocInfo.commandPool = m_context.graphicsCmdPool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;

		return m_context.logicalDevice.allocateCommandBuffers(allocInfo)[0];
	}

	vk::CommandBuffer CommandBuffers::AllocateComputeCmdBuffer()
	{
		vk::CommandBufferAllocateInfo allocInfo = {};
		allocInfo.commandBufferCount = 1;
		allocInfo.commandPool = m_context.computeCmdPool;
		allocInfo.level = vk::CommandBufferLevel::ePrimary;

		return m_context.logicalDevice.allocateCommandBuffers(allocInfo)[0];

	}

	void CommandBuffers::RecordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex,const SwapchainResources& swapchainResources,
		vk::Extent2D swapchainExtent, vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, uint32_t vertexCount)
	{
		vk::CommandBufferBeginInfo begineInfo{};
		commandBuffer.begin(begineInfo);

		vk::ClearValue clearColour;
		clearColour.color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

		vk::RenderPassBeginInfo rbBeginInfo{};
		rbBeginInfo.renderPass = swapchainResources.renderPass.handle;
		rbBeginInfo.clearValueCount = 1;
		rbBeginInfo.framebuffer = swapchainResources.framebuffers[imageIndex].handle;
		rbBeginInfo.renderArea.offset = vk::Offset2D{ 0,0 };
		rbBeginInfo.renderArea.extent = swapchainExtent;
		rbBeginInfo.pClearValues = &clearColour;

		commandBuffer.beginRenderPass(rbBeginInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;

		viewport.height = static_cast<float>(swapchainExtent.height);
		viewport.width = static_cast<float>(swapchainExtent.width);

		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		commandBuffer.setViewport(0, 1, &viewport);

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{ 0,0 };
		scissor.extent = swapchainExtent;
		commandBuffer.setScissor(0, 1, &scissor);

		vk::Buffer vertexBuffers[] = { vertexBuffer };
		vk::DeviceSize offsets[] = { 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

		commandBuffer.draw(vertexCount, 1, 0, 0);

		commandBuffer.endRenderPass();

		commandBuffer.end();

	}


}
