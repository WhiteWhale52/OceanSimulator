#pragma once
#include <VulkanCore/Config/CommonHeaders.h>

namespace TheRenderer::Vulkan {

	constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;

	struct FrameData {
		vk::CommandBuffer commandBuffer;
		vk::Semaphore imageAvailableSemaphore;
		vk::Semaphore renderFinishedSemaphone;
		vk::Fence inFlightFence;
	};

}