#include "CommandBuffers.h"

namespace Renderer::Vulkan {

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


}
