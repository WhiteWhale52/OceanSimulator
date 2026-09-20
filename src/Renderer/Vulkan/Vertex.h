#pragma once
#include <VulkanCore/Config/CommonHeaders.h>

namespace Renderer::Vulkan {
	struct Vertex {
		glm::vec3 position;
		glm::vec4 colour;

		static vk::VertexInputBindingDescription GetBindingDescription() {
			vk::VertexInputBindingDescription binding{};
			binding.binding = 0;
			binding.stride = sizeof(Vertex);
			binding.inputRate = vk::VertexInputRate::eVertex;
			return binding;
		}
		static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescriptions() {
			std::vector<vk::VertexInputAttributeDescription> attributes(2);

			attributes[0].binding = 0;
			attributes[0].location = 0;
			attributes[0].format = vk::Format::eR32G32B32Sfloat;
			attributes[0].offset = offsetof(Vertex, position);

			attributes[1].binding = 0;
			attributes[1].location = 1;
			attributes[1].format = vk::Format::eR32G32B32A32Sfloat;
			attributes[1].offset = offsetof(Vertex, colour);

			return attributes;
		}

	};
}