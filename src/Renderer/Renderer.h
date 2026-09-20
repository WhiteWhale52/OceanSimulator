#pragma once
#include <VulkanCore/Config/CommonHeaders.h>
#include <VulkanCore/VulkanContext.h>
#include "Vulkan/CommandBuffers.h"
#include "Vulkan/Pipelines.h"
#include "Vulkan/VulkanResources.h"
#include "Vulkan/SwapchainResources.h"
#include "FrameResources.h"

namespace TheRenderer {
	class Renderer {
	private:
		Core::Vulkan::VulkanContext& m_context;

		Vulkan::CommandBuffers m_commandBuffers;
		Vulkan::Buffer m_vertexBuffer;
		Vulkan::Pipeline m_trianglePipeline;
		Vulkan::SwapchainResources m_swapchainResources;

		std::array<Vulkan::FrameData, Vulkan::MAX_FRAMES_IN_FLIGHT> m_frames;
		uint32_t m_currentFrame;


		void CreateFrameData();
		void CreateTrianglePipeline();
		void CreateTriangleVertexBuffer();

	public:
		Renderer(Core::Vulkan::VulkanContext& context);
		~Renderer();
		void Init();
		void DrawFrame();
	};
}