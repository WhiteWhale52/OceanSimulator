#include "Renderer.h"
#include "Vulkan/TriangleSetup.h"

namespace TheRenderer {

	void Renderer::CreateTriangleVertexBuffer()
	{
		m_vertexBuffer.CreateVertexBuffer(m_context, Vulkan::triangleVertices.data(),
			sizeof(Vulkan::Vertex) * Vulkan::triangleVertices.size());

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
			frame.renderFinishedSemaphone = m_context.logicalDevice.createSemaphore(semaphoreCreateInfo);
			frame.inFlightFence = m_context.logicalDevice.createFence(fenceCreateInfo);
		}
	}
	
	Renderer::Renderer(Core::Vulkan::VulkanContext& context) : m_context(context), m_commandBuffers(context)
	{

	}

	
	
	
	Renderer::~Renderer()
	{
	}

	void Renderer::Init(GLFWwindow* window)
	{
		m_swapchain.CreateSwapchain(m_context, window);
		m_renderPass.Create(m_context, m_swapchain.GetImageFormat(), m_swapchain.GetDepthFormat());
		m_swapchain.CreateFramebuffers(m_context, m_renderPass);
		CreateTriangleVertexBuffer();
		CreateTrianglePipeline();
		CreateFrameData();
	}

	void Renderer::DrawFrame()
	{
	}



}

