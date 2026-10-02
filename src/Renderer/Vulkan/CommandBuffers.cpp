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

	//void CommandBuffers::RecordCommandBuffer(vk::CommandBuffer commandBuffer, vk::RenderPass renderPass, vk::Framebuffer framebuffer, vk::Extent2D extent,
	//	vk::Pipeline graphicsPipeline, vk::PipelineLayout pipelineLayout, vk::DescriptorSet descriptorSet, vk::Buffer vertexBuffer, vk::Buffer indexBuffer,
	//	uint32_t indexCount, uint32_t vertexCount)
	//{
	//	vk::CommandBufferBeginInfo begineInfo{};
	//	commandBuffer.begin(begineInfo);

	//	std::array < vk::ClearValue, 2 >clearValues{};
	//	clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
	//	clearValues[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

	//	vk::RenderPassBeginInfo rbBeginInfo{};
	//	rbBeginInfo.renderPass = renderPass;
	//	rbBeginInfo.clearValueCount = clearValues.size();
	//	rbBeginInfo.framebuffer = framebuffer;
	//	rbBeginInfo.renderArea.offset = vk::Offset2D{ 0,0 };
	//	rbBeginInfo.renderArea.extent = extent;
	//	rbBeginInfo.pClearValues = clearValues.data();

	//	commandBuffer.beginRenderPass(rbBeginInfo, vk::SubpassContents::eInline);
	//	commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);
	//	commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

	//	vk::Viewport viewport{};
	//	viewport.x = 0.0f;
	//	viewport.y = 0.0f;

	//	viewport.height = static_cast<float>(extent.height);
	//	viewport.width = static_cast<float>(extent.width);

	//	viewport.minDepth = 0.0f;
	//	viewport.maxDepth = 1.0f;
	//	commandBuffer.setViewport(0, 1, &viewport);

	//	vk::Rect2D scissor{};
	//	scissor.offset = vk::Offset2D{ 0,0 };
	//	scissor.extent = extent;
	//	commandBuffer.setScissor(0, 1, &scissor);

	//	vk::Buffer vertexBuffers[] = { vertexBuffer };
	//	vk::DeviceSize offsets[] = { 0 };
	//	commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);
	//	commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint32);

	//	commandBuffer.drawIndexed(indexCount, 1, 0, 0, 0);


	//	//commandBuffer.draw(vertexCount, 1, 0, 0);

	//	commandBuffer.endRenderPass();

	//	commandBuffer.end();

	//}


	void RecordCommandBuffer(vk::CommandBuffer commandBuffer, vk::RenderPass renderPass, vk::Framebuffer framebuffer, vk::Extent2D extent,
		vk::Pipeline graphicsPipeline, vk::Buffer vertexBuffer, uint32_t vertexCount) {
		vk::CommandBufferBeginInfo beginInfo{};
		commandBuffer.begin(beginInfo);

		std::array<vk::ClearValue, 2> clearValues{};
		clearValues[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
		clearValues[1].depthStencil = vk::ClearDepthStencilValue{ 1.0f, 0 };

		vk::RenderPassBeginInfo rbBeginInfo{};
		rbBeginInfo.renderPass = renderPass;
		rbBeginInfo.framebuffer = framebuffer;
		rbBeginInfo.renderArea.offset = vk::Offset2D{ 0, 0 };
		rbBeginInfo.renderArea.extent = extent;
		rbBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		rbBeginInfo.pClearValues = clearValues.data();

		commandBuffer.beginRenderPass(rbBeginInfo, vk::SubpassContents::eInline);
		commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

		vk::Viewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		commandBuffer.setViewport(0, 1, &viewport);

		vk::Rect2D scissor{};
		scissor.offset = vk::Offset2D{ 0, 0 };
		scissor.extent = extent;
		commandBuffer.setScissor(0, 1, &scissor);

		vk::Buffer vertexBuffers[] = { vertexBuffer };
		vk::DeviceSize offsets[] = { 0 };
		commandBuffer.bindVertexBuffers(0, 1, vertexBuffers, offsets);

		commandBuffer.draw(vertexCount, 1, 0, 0);

		commandBuffer.endRenderPass();
		commandBuffer.end();
	}
}
