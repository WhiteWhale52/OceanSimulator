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

	void Renderer::Vulkan::ShaderModule::LoadSPIRV(const Core::Vulkan::VulkanContext& context, const std::string& filename)
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

	Pipeline CreateComputePipeline(Core::Vulkan::VulkanContext& context, const ComputePipelineConfig& computePipeConfig)
	{
		return Pipeline();
	}

	Pipeline CreateGraphicsPipeline(Core::Vulkan::VulkanContext& context, const GraphicsPipelineConfig& graphicsPipeConfig)
	{
		Pipeline graphicsPipeline;

		ShaderModule vertShader = ShaderModule();
		vertShader.LoadSPIRV(context, graphicsPipeConfig.vertShader);

		ShaderModule fragShader = ShaderModule();
		fragShader.LoadSPIRV(context, graphicsPipeConfig.fragShader);

		if (!graphicsPipeConfig.tesShader.empty()) {
			ShaderModule tesShader = ShaderModule();
			tesShader.LoadSPIRV(context, graphicsPipeConfig.tesShader);
		}

		std::vector<vk::PipelineShaderStageCreateInfo> stages;
		
		vk::PipelineShaderStageCreateInfo vertShaderCreateInfo;
		vertShaderCreateInfo.stage = vk::

		return  graphicsPipeline;
	}

	void Pipeline::DestroyPipeline(Core::Vulkan::VulkanContext context, Pipeline& pipeline)
	{}

}