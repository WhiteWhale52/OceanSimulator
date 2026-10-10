#include "TheRenderer.h"
#include "Vulkan/TriangleSetup.h"


namespace TheRenderer {

	Renderer::Renderer(Core::Vulkan::VulkanContext& context) : m_context(context), m_commandBuffers(context), m_currentFrame(0), m_desciptors(context)
	{
	}

	void Renderer::Init(GLFWwindow* window)
	{
		m_window = window;

		m_swapchain.CreateSwapchain(m_context, window);
		m_renderPass.Create(m_context, m_swapchain.GetImageFormat(), m_swapchain.GetDepthFormat());
		m_swapchain.CreateFramebuffers(m_context, m_renderPass);
		CreateRenderFinishedSemaphores();

		CreateMesh();
		CreateCameraResources();
		//CreateTriangleVertexBuffer();
		CreateTrianglePipeline();
		CreateFrameData();

	}

	void Renderer::CreateTriangleVertexBuffer()
	{
		m_vertexBuffer.CreateVertexBuffer(m_context, Vulkan::triangleVertices.data(),
			sizeof(Vulkan::Vertex) * Vulkan::triangleVertices.size());
	}

	void Renderer::RecreateSwapchain()
	{
		m_context.logicalDevice.waitIdle();
		DestroyRenderFinishedSemaphores();
		m_swapchain.RecreateSwapChain(m_context, m_window, m_renderPass);
		CreateRenderFinishedSemaphores();
	}

	void Renderer::CreateTrianglePipeline() {
		Vulkan::GraphicsPipelineConfig config{};
		config.vertShader = "C:/Users/moham/Documents/GitHub/OceanSimulator/src/Renderer/Shaders/triangle.vert.spv";
		config.fragShader = "C:/Users/moham/Documents/GitHub/OceanSimulator/src/Renderer/Shaders/triangle.frag.spv";
		config.topology = vk::PrimitiveTopology::eTriangleList;
		config.cullMode = vk::CullModeFlagBits::eNone;
		config.polygonMode = vk::PolygonMode::eLine;
		config.frontface = vk::FrontFace::eCounterClockwise;
		config.depthTest = false;
		config.depthWrite = false;
		config.renderPass = m_renderPass.handle;
		config.vertexBindingDescription = Vulkan::Vertex::GetBindingDescription();
		config.vertexAttributeDecriptions = Vulkan::Vertex::GetAttributeDescriptions();
		config.descriptorLayout = m_desciptors.cameraDescriptorLayout;

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
			frame.inFlightFence = m_context.logicalDevice.createFence(fenceCreateInfo);
		}
	}
	

	
	
	
	Renderer::~Renderer()
	{
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

		UpdateCameraUBO(m_currentFrame);

		frame.commandBuffer.reset();
		m_commandBuffers.RecordCommandBuffer(frame.commandBuffer, m_renderPass.handle, m_swapchain.GetFrameBuffers()[imageIndex].handle,
			m_swapchain.GetExtent(), m_trianglePipeline.handle, m_trianglePipeline.layout, m_cameraDescriptorSets[m_currentFrame], m_vertexBuffer.handle, m_indexBuffer.handle,
			m_indexCount);

		//m_commandBuffers.RecordCommandBuffer(frame.commandBuffer, m_renderPass.handle, m_swapchain.GetFrameBuffers()[imageIndex].handle,
		//	m_swapchain.GetExtent(), m_trianglePipeline.handle, m_vertexBuffer.handle, static_cast<uint32_t>(Vulkan::triangleVertices.size()));

		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

		vk::SubmitInfo submitInfo{};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &frame.imageAvailableSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &frame.commandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_renderFinishedSemaphores[imageIndex];

		m_context.graphicsQueue.submit(submitInfo, frame.inFlightFence);

		vk::PresentInfoKHR presentInfo{};
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &m_renderFinishedSemaphores[imageIndex];
		presentInfo.swapchainCount = 1;
		vk::SwapchainKHR swapchainHandle = m_swapchain.GetSwapChain();
		presentInfo.pSwapchains = &swapchainHandle;
		presentInfo.pImageIndices = &imageIndex;

		vk::Result presentResult = m_context.presentQueue.presentKHR(&presentInfo);

		if (presentResult == vk::Result::eErrorOutOfDateKHR || presentResult == vk::Result::eSuboptimalKHR || m_framebufferResized) {
			m_framebufferResized = false;
			RecreateSwapchain();
		}
		else if (presentResult != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to present swap chain image!");
		}

		m_currentFrame = (m_currentFrame + 1) % Vulkan::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::CreateRenderFinishedSemaphores() {
		vk::SemaphoreCreateInfo createInfo{};
		m_renderFinishedSemaphores.resize(m_swapchain.GetImages().size());
		for (auto& semaphore : m_renderFinishedSemaphores) {
			semaphore = m_context.logicalDevice.createSemaphore(createInfo);
		}
	}
	void Renderer::DestroyRenderFinishedSemaphores() {
		for (auto& sem : m_renderFinishedSemaphores)
			m_context.logicalDevice.destroySemaphore(sem);
		m_renderFinishedSemaphores.clear();
	}

	void Renderer::CreateMesh()
	{
		Vulkan::MeshData mesh = Vulkan::GenerateMesh(64, 1.5f);
		m_indexCount = static_cast<uint32_t>(mesh.indices.size());

		m_vertexBuffer.CreateVertexBuffer(m_context, mesh.vertices.data(), sizeof(Vulkan::Vertex) * mesh.vertices.size());
		m_indexBuffer.CreateIndexBuffer(m_context, mesh.indices.data(), sizeof(uint32_t) * mesh.indices.size());
		

	}

	void Renderer::Shutdown()
	{
		m_context.logicalDevice.waitIdle();

		for (auto& frame : m_frames) {
			m_context.logicalDevice.destroySemaphore(frame.imageAvailableSemaphore);
			m_context.logicalDevice.destroyFence(frame.inFlightFence);
		}
		DestroyRenderFinishedSemaphores();

		for (auto& ubo : m_cameraBuffers)
			vmaDestroyBuffer(m_context.vmaAllocator, static_cast<VkBuffer>(ubo.handle), ubo.allocation);

		m_context.logicalDevice.destroyDescriptorPool(m_desciptors.pool);   // also frees the sets allocated from it
		m_context.logicalDevice.destroyDescriptorSetLayout(m_desciptors.cameraDescriptorLayout);

		m_vertexBuffer.Destroy(m_context);
		m_indexBuffer.Destroy(m_context);
		m_trianglePipeline.Destroy(m_context);
		m_swapchain.Destroy(m_context);
		m_renderPass.Destroy(m_context);
	}



	void Renderer::CreateCameraResources()
	{
		m_desciptors.CreateCameraDescriptorSetLayout();
		m_desciptors.CreateDescriptorPool(Vulkan::MAX_FRAMES_IN_FLIGHT);
		m_cameraDescriptorSets = m_desciptors.AllocateCameraDescriptorSets(Vulkan::MAX_FRAMES_IN_FLIGHT);

		for (uint32_t i = 0; i < Vulkan::MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkBufferCreateInfo bufferInfo{};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = sizeof(Vulkan::CameraUBO);
			bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VmaAllocationCreateInfo allocInfo{};
			allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
			allocInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

			VkBuffer rawBuffer;
			VmaAllocationInfo outInfo{};
			if (vmaCreateBuffer(m_context.vmaAllocator, &bufferInfo, &allocInfo, &rawBuffer, &m_cameraBuffers[i].allocation, &outInfo) != VK_SUCCESS) {
				throw std::runtime_error("Failed to create camera UBO");
			}

			m_cameraBuffers[i].handle = rawBuffer;
			m_cameraBuffers[i].mapped = outInfo.pMappedData;

			vk::DescriptorBufferInfo bufferDescriptorInfo{};
			bufferDescriptorInfo.buffer = m_cameraBuffers[i].handle;
			bufferDescriptorInfo.offset = 0;
			bufferDescriptorInfo.range = sizeof(Vulkan::CameraUBO);

			vk::WriteDescriptorSet writeSet{};
			writeSet.dstSet = m_cameraDescriptorSets[i];
			writeSet.dstBinding = 0;
			writeSet.dstArrayElement = 0;
			writeSet.descriptorType = vk::DescriptorType::eUniformBuffer;
			writeSet.descriptorCount = 1;
			writeSet.pBufferInfo = &bufferDescriptorInfo;

			m_context.logicalDevice.updateDescriptorSets(1, &writeSet, 0, nullptr);
		}

	}

	void Renderer::UpdateCameraUBO(uint32_t frameIndex)
	{
		Vulkan::CameraUBO cameraUBO{};
		cameraUBO.view = glm::lookAt(glm::vec3(0.0f, 1.5f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		cameraUBO.proj = glm::perspective(glm::radians(45.0f), m_swapchain.GetExtent().width / static_cast<float>(m_swapchain.GetExtent().height), 0.1f, 100.0f);

		cameraUBO.proj[1][1] *= -1;
		std::memcpy(m_cameraBuffers[frameIndex].mapped, &cameraUBO, sizeof(cameraUBO));
	}

}

