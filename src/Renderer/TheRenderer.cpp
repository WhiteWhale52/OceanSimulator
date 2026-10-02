#include "TheRenderer.h"
#include "Vulkan/TriangleSetup.h"

namespace TheRenderer {

	Renderer::Renderer(Core::Vulkan::VulkanContext& context) : m_context(context), m_commandBuffers(context)
	{
	}

	void Renderer::CreateTriangleVertexBuffer()
	{
		m_vertexBuffer.CreateVertexBuffer(m_context, Vulkan::triangleVertices.data(),
			sizeof(Vulkan::Vertex) * Vulkan::triangleVertices.size());
	}

	void Renderer::RecreateSwapchain()
	{
		m_swapchain.RecreateSwapChain(m_context, m_window, m_renderPass);
	}

	void Renderer::CreateTrianglePipeline() {
		Vulkan::GraphicsPipelineConfig config{};
		config.vertShader = "Shaders/triangle.vert.spv";
		config.fragShader = "Shaders/triangle.frag.spv";
		config.topology = vk::PrimitiveTopology::eTriangleList;
		config.cullMode = vk::CullModeFlagBits::eNone;
		config.polygonMode = vk::PolygonMode::eFill;
		config.frontface = vk::FrontFace::eCounterClockwise;
		config.depthTest = false;
		config.depthWrite = false;
		config.renderPass = m_renderPass.handle;
		config.vertexBindingDescription = Vulkan::Vertex::GetBindingDescription();
		config.vertexAttributeDecriptions = Vulkan::Vertex::GetAttributeDescriptions();

		vk::PipelineCache pipelineCache = nullptr;
		m_trianglePipeline = Vulkan::CreateGraphicsPipeline(m_context, config, pipelineCache);
	}

	void Renderer::CreateFrameData()
	{
		vk::SemaphoreCreateInfo semaphoreCreateInfo{};
		vk::FenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.flags = vk::FenceCreateFlagBits::eSignaled;

		for (auto& frame : m_frames) {
			frame.commandBuffer = m_commandBuffers.AllocateGraphicsCmdBuffer();
			frame.imageAvailableSemaphore = m_context.logicalDevice.createSemaphore(semaphoreCreateInfo);
			frame.renderFinishedSemaphore = m_context.logicalDevice.createSemaphore(semaphoreCreateInfo);
			frame.inFlightFence = m_context.logicalDevice.createFence(fenceCreateInfo);
		}
	}
	

	
	
	
	Renderer::~Renderer()
	{
	}

	void Renderer::Init(GLFWwindow* window)
	{
		m_window = window;

		m_swapchain.CreateSwapchain(m_context, window);
		m_renderPass.Create(m_context, m_swapchain.GetImageFormat(), m_swapchain.GetDepthFormat());
		m_swapchain.CreateFramebuffers(m_context, m_renderPass);

		CreateTriangleVertexBuffer();
		CreateTrianglePipeline();
		CreateFrameData();
	}

	void Renderer::DrawFrame()
	{
		auto& frame = m_frames[m_currentFrame];
		auto& device = m_context.logicalDevice;

		if (device.waitForFences(frame.inFlightFence, VK_TRUE, UINT64_MAX) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to wait for fence!");
		}

		uint32_t imageIndex;
		vk::Result acquire = device.acquireNextImageKHR(m_swapchain.GetSwapChain(), UINT64_MAX, frame.imageAvailableSemaphore, nullptr, &imageIndex);

		if (acquire == vk::Result::eErrorOutOfDateKHR) {
			RecreateSwapchain();
			return;
		}
		else if (acquire != vk::Result::eSuccess && acquire != vk::Result::eSuboptimalKHR) {
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		device.resetFences(frame.inFlightFence);

		frame.commandBuffer.reset();
		//m_commandBuffers.RecordCommandBuffer(frame.commandBuffer, m_renderPass.handle, m_swapchain.GetFrameBuffers()[imageIndex].handle,
		//	m_swapchain.GetExtent(), m_trianglePipeline.handle, m_trianglePipelineLayout, m_descriptorSet, m_vertexBuffer.handle, m_indexBuffer.handle,
		//	static_cast<uint32_t>(triangleIndices.size()), static_cast<uint32_t>(triangleVertices.size()));

		m_commandBuffers.RecordCommandBuffer(frame.commandBuffer, m_renderPass.handle, m_swapchain.GetFrameBuffers()[imageIndex].handle,
			m_swapchain.GetExtent(), m_trianglePipeline.handle, m_vertexBuffer.handle, static_cast<uint32_t>(Vulkan::triangleVertices.size()));

		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		vk::SubmitInfo submitInfo{};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &frame.imageAvailableSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frame.commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &frame.renderFinishedSemaphore;

		m_context.graphicsQueue.submit(submitInfo, frame.inFlightFence);

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &frame.renderFinishedSemaphore;
		presentInfo.swapchainCount = 1;
		vk::SwapchainKHR swapchainHandle = m_swapchain.GetSwapChain();
		presentInfo.pSwapchains = &swapchainHandle;
		presentInfo.pImageIndices = &imageIndex;

		vk::Result presentResult = m_context.presentQueue.presentKHR(presentInfo);

		if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || m_framebufferResized) {
			m_framebufferResized = false;
			RecreateSwapchain();
		}
		else if (presentResult != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		m_currentFrame = (m_currentFrame + 1) % Vulkan::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::Shutdown()
	{
		m_context.logicalDevice.waitIdle();

		for (auto& frame : m_frames){
			m_context.logicalDevice.destroySemaphore(frame.imageAvailableSemaphore);
			m_context.logicalDevice.destroySemaphore(frame.renderFinishedSemaphore);
			m_context.logicalDevice.destroyFence(frame.inFlightFence);
		}

		m_vertexBuffer.Destroy(m_context);
		m_indexBuffer.Destroy(m_context);
		m_trianglePipeline.Destroy(m_context);
		m_swapchain.Destroy(m_context);
		m_renderPass.Destroy(m_context);

	}



}

