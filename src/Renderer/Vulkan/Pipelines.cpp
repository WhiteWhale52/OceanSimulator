#include "Pipelines.h"

namespace Renderer::Vulkan {

	
	static std::vector<uint32_t> ReadSPIRVFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
#if DEBUG_VULKAN
			logger.print("Failed to open shader file: ", filename);
#endif
			throw std::runtime_error("Failed to open shader file: " + filename);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<uint32_t> buffer(fileSize / sizeof(size_t));

		file.seekg(0);
		file.read((char*)buffer.data(), fileSize);
		file.close();
		return buffer;

	}

	void ShaderModule::LoadSPIRV(const Core::Vulkan::VulkanContext& context, const std::string& filename)
	{
		auto spirv = ReadSPIRVFile(filename);

		vk::ShaderModuleCreateInfo createInfo;
		createInfo.codeSize = spirv.size()* sizeof(uint32_t);
		createInfo.pCode = spirv.data();

		context.logicalDevice.createShaderModule(&createInfo, nullptr, &handle);

		
		
	}

	void ShaderModule::Destroy(Core::Vulkan::VulkanContext context) {
		if (handle != VK_NULL_HANDLE) {
			context.logicalDevice.destroyShaderModule(handle);
		}
	}


	
	Pipeline::ReflectedLayout Pipeline::ReflectSPIRV(const std::vector<uint32_t>& spirv, vk::ShaderStageFlagBits stageBits)
	{
		return Pipeline::ReflectedLayout();
	}

	void Pipeline::MergeReflectedLayouts(Pipeline::ReflectedLayout& dst, const Pipeline::ReflectedLayout& src)
	{}

	vk::PipelineLayout Pipeline::BuildPipelineLayout(Core::Vulkan::VulkanContext& context, const Pipeline::ReflectedLayout& layout, std::vector<vk::DescriptorSetLayout>& outSetLayouts)
	{
		return vk::PipelineLayout();
	}

	Pipeline CreateComputePipeline(Core::Vulkan::VulkanContext& context, const ComputePipelineConfig& computePipeConfig,
		 vk::PipelineCache& pipelineCache)
	{
		return Pipeline();
	}

	Pipeline CreateGraphicsPipeline(Core::Vulkan::VulkanContext& context,  GraphicsPipelineConfig& graphicsPipeConfig,
		 vk::PipelineCache& pipelineCache)
	{
		Pipeline graphicsPipeline;
		std::vector<vk::PipelineShaderStageCreateInfo> stages;

		ShaderModule vertShader = ShaderModule();
		vertShader.LoadSPIRV(context, graphicsPipeConfig.vertShader);

		ShaderModule fragShader = ShaderModule();
		fragShader.LoadSPIRV(context, graphicsPipeConfig.fragShader);

		vk::PipelineShaderStageCreateInfo vertStageCreateInfo;
		vertStageCreateInfo.stage = vk::ShaderStageFlagBits::eVertex;
		vertStageCreateInfo.module = vertShader.handle;
		vertStageCreateInfo.pName = "main";
		stages.push_back(vertStageCreateInfo);

		vk::PipelineShaderStageCreateInfo fragStageCreateInfo;
		fragStageCreateInfo.stage = vk::ShaderStageFlagBits::eFragment;
		fragStageCreateInfo.module = fragShader.handle;
		fragStageCreateInfo.pName = "main";
		stages.push_back(fragStageCreateInfo);

		if (!graphicsPipeConfig.tesControlShader.empty() && !graphicsPipeConfig.tesEvalShader.empty()) {
			
			ShaderModule tesControlShader = ShaderModule();
			tesControlShader.LoadSPIRV(context, graphicsPipeConfig.tesControlShader);
			
			ShaderModule tesEvalShader = ShaderModule();
			tesEvalShader.LoadSPIRV(context, graphicsPipeConfig.tesEvalShader);
			
			vk::PipelineShaderStageCreateInfo tesControlStageCreateInfo;
			tesControlStageCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationControl;
			tesControlStageCreateInfo.module = fragShader.handle;
			tesControlStageCreateInfo.pName = "main";
			stages.push_back(tesControlStageCreateInfo);
			
			vk::PipelineShaderStageCreateInfo tesEvalStageCreateInfo;
			tesEvalStageCreateInfo.stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
			tesEvalStageCreateInfo.module = tesEvalShader.handle;
			tesEvalStageCreateInfo.pName = "main";
			stages.push_back(tesEvalStageCreateInfo);
		}

		if (!graphicsPipeConfig.geomShader.empty() ) {

			ShaderModule geomShader = ShaderModule();
			geomShader.LoadSPIRV(context, graphicsPipeConfig.geomShader);

			vk::PipelineShaderStageCreateInfo geomStageCreateInfo;
			geomStageCreateInfo.stage = vk::ShaderStageFlagBits::eGeometry;
			geomStageCreateInfo.module = geomShader.handle;
			geomStageCreateInfo.pName = "main";
			stages.push_back(geomStageCreateInfo);
			
		}


		vk::PipelineVertexInputStateCreateInfo vertexInput;
		vertexInput.vertexAttributeDescriptionCount = 0;
		vertexInput.vertexBindingDescriptionCount = 0;

		vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
		inputAssembly.topology = graphicsPipeConfig.topology;
		inputAssembly.primitiveRestartEnable = vk::False;
		
		vk::Viewport viewPort;
		viewPort.height = 600.0f;
		viewPort.minDepth = 0.0f;
		viewPort.maxDepth = 1.0f;
		viewPort.width = 800.0f;
		viewPort.x = 0.0f;
		viewPort.y = 0.0f;

		vk::Rect2D scissorRect;
		scissorRect.extent = vk::Extent2D{ 800,600 };
		scissorRect.offset = vk::Offset2D{ 0,0 };

		vk::PipelineViewportStateCreateInfo viewPortState;
		viewPortState.scissorCount = 1;
		viewPortState.viewportCount = 1;
		viewPortState.pScissors = &scissorRect;
		viewPortState.pViewports = &viewPort;

		vk::PipelineRasterizationStateCreateInfo rasterization;
		rasterization.cullMode = graphicsPipeConfig.cullMode;
		rasterization.polygonMode = graphicsPipeConfig.polygonMode;
		rasterization.depthBiasClamp = vk::False;
		rasterization.frontFace = graphicsPipeConfig.frontface;
		rasterization.lineWidth = 1.0f;
		rasterization.rasterizerDiscardEnable = vk::False;

		vk::PipelineMultisampleStateCreateInfo multisampling;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
		multisampling.sampleShadingEnable = vk::False;

		vk::PipelineDepthStencilStateCreateInfo depthStencil;
		depthStencil.depthTestEnable = graphicsPipeConfig.depthTest;
		depthStencil.depthWriteEnable = graphicsPipeConfig.depthWrite;
		depthStencil.depthCompareOp = vk::CompareOp::eLessOrEqual;
		depthStencil.depthBoundsTestEnable = vk::False;
		depthStencil.stencilTestEnable = vk::False;

		vk::PipelineColorBlendAttachmentState colorBlendAttachment;
		colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

		vk::PipelineColorBlendStateCreateInfo colorBlend;
		colorBlend.attachmentCount = 1;
		colorBlend.logicOpEnable = vk::False;
		colorBlend.pAttachments = &colorBlendAttachment;

		std::vector<vk::DynamicState> dynamicStates;
		dynamicStates.push_back(vk::DynamicState::eViewport);
		dynamicStates.push_back(vk::DynamicState::eScissor);

		vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo;
		dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

		vk::PipelineLayoutCreateInfo pipelineLayout;
		pipelineLayout.setLayoutCount = graphicsPipeConfig.descriptorLayout ? 1 : 0;
		if (graphicsPipeConfig.descriptorLayout) {
			pipelineLayout.pSetLayouts = &graphicsPipeConfig.descriptorLayout;
		}
		else {
			pipelineLayout.pSetLayouts =  nullptr;
		}
		
		if (context.logicalDevice.createPipelineLayout(&pipelineLayout, nullptr, &graphicsPipeline.layout) 
			!= vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create pipeline layout");
		}
		vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
		graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(stages.size());
		graphicsPipelineCreateInfo.pStages = stages.data();
		graphicsPipelineCreateInfo.pColorBlendState = &colorBlend;
		graphicsPipelineCreateInfo.pDepthStencilState = &depthStencil;
		graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
		graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssembly;
		graphicsPipelineCreateInfo.pVertexInputState = &vertexInput;
		graphicsPipelineCreateInfo.pMultisampleState = &multisampling;
		graphicsPipelineCreateInfo.pViewportState = &viewPortState;
		graphicsPipelineCreateInfo.pRasterizationState = &rasterization;
		graphicsPipelineCreateInfo.renderPass = graphicsPipeConfig.renderPass;
		graphicsPipelineCreateInfo.layout = graphicsPipeline.layout;
		graphicsPipelineCreateInfo.subpass = 0;

		context.logicalDevice.createGraphicsPipeline(pipelineCache, graphicsPipelineCreateInfo, nullptr,
			graphicsPipeline.handle);

		return  graphicsPipeline;


	}

	void Pipeline::DestroyPipeline(Core::Vulkan::VulkanContext context, Pipeline& pipeline)
	{
		if (pipeline.handle) {
			context.logicalDevice.destroyPipeline(handle);
			context.logicalDevice.destroyPipelineLayout(layout);
		}
	}

}