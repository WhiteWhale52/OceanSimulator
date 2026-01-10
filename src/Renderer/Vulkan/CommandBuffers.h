#pragma once

#include <vulkan/vulkan.hpp>
#include <Core/Vulkan/VulkanContext.h>
#include <Cor>
 

namespace Renderer::CommandBuffers {
	class CommandBuffer {
	public:
		CommandBuffer(Core::Vulkan::VulkanContext& context);
	};
}
