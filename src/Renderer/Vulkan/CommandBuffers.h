#pragma once

#include <vulkan/vulkan.hpp>
#include <Vulkan/VulkanContext.h> 

namespace Renderer::Vulkan {
	class CommandBuffers {
	public:
		CommandBuffers(Core::Vulkan::VulkanContext& context);
	
		vk::CommandBuffer AllocateGraphicsCmdBuffer();
		vk::CommandBuffer AllocateComputeCmdBuffer();
	private:
		Core::Vulkan::VulkanContext& m_context;
	};
}
