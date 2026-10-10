#pragma once
#include <VulkanCore/Config/CommonHeaders.h>

namespace TheRenderer::Vulkan {
	struct CameraUBO {
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};
}