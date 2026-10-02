#pragma once
#include <VulkanCore/Config/CommonHeaders.h>
#include <VulkanCore/VulkanContext.h>
#include "Vulkan/CommandBuffers.h"
#include "Vulkan/Pipelines.h"
#include "Vulkan/VulkanResources.h"
#include "Vulkan/Swapchain.h"
#include "FrameResources.h"

namespace TheRenderer {
	class Renderer {
	private:
		Core::Vulkan::VulkanContext& m_context;
		GLFWwindow* m_window = nullptr;
		
		Vulkan::CommandBuffers m_commandBuffers;
		Vulkan::Buffer m_vertexBuffer;
		Vulkan::Buffer m_indexBuffer;
		Vulkan::Pipeline m_trianglePipeline;
		vk::PipelineLayout m_trianglePipelineLayout;
		vk::DescriptorSet m_descriptorSet;
		Vulkan::Swapchain m_swapchain;
		Vulkan::RenderPass m_renderPass;

		std::array<Vulkan::FrameData, Vulkan::MAX_FRAMES_IN_FLIGHT> m_frames;
		uint32_t m_currentFrame = 0;
		bool m_framebufferResized = false;

		void CreateFrameData();
		void CreateTrianglePipeline();
		void CreateTriangleVertexBuffer();
		void RecreateSwapchain();

	public:
		Vulkan::CommandBuffers GetCommandBuffers() { return m_commandBuffers; }
		Vulkan::Buffer GetVertexBuffer() { return m_vertexBuffer; }
		Vulkan::Pipeline GetTrianglePipeline() { return m_trianglePipeline; }
		Vulkan::Swapchain GetSwapchain() { return m_swapchain; }
		Vulkan::RenderPass GetRenderPass() { return m_renderPass; }

		Renderer(Core::Vulkan::VulkanContext& context);
		~Renderer();

		void Init(GLFWwindow* window);
		void DrawFrame();
		void Shutdown();
		void NotifyResized() { m_framebufferResized = true; }
	};
}