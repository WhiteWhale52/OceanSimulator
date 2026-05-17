#pragma once

#include <VulkanCore/VulkanContext.h>
#include <spirv_reflect.h>
#include <filesystem>
#include <fstream>


namespace Renderer::Vulkan {
   
    struct GraphicsPipelineConfig {
        std::string              vertShader;
        std::string              fragShader;
        std::string              tesControlShader;     // Optional
        std::string              tesEvalShader;     // Optional
        std::string              geomShader;     // Optional
        vk::PrimitiveTopology    topology = vk::PrimitiveTopology::eTriangleList;
        vk::CullModeFlagBits     cullMode = vk::CullModeFlagBits::eBack;
        vk::FrontFace            frontface = vk::FrontFace::eCounterClockwise;
        vk::PolygonMode          polygonMode = vk::PolygonMode::eFill;
        bool                     depthTest = true;
        bool                     depthWrite = true;
        bool                     blendEnable = false;
        uint32_t                 patchPoints = 3;
        vk::RenderPass           renderPass = VK_NULL_HANDLE;
        vk::DescriptorSetLayout  descriptorLayout = VK_NULL_HANDLE;
        const char* debugName = nullptr;
    };

    struct ComputePipelineConfig {
        std::string              computeShader;
        vk::DescriptorSetLayout    descriptorLayout = VK_NULL_HANDLE;
        const char* debugName = nullptr;
    };


	struct Pipeline {
		vk::Pipeline handle = VK_NULL_HANDLE;

		vk::PipelineLayout layout;

		std::vector<vk::DescriptorSetLayout> descSetLayout;

		vk::PipelineBindPoint bindPoint = vk::PipelineBindPoint::eGraphics;
        struct PipelineCache {
            vk::PipelineCache handle = VK_NULL_HANDLE;

            std::string cachePath;

        };

        struct ReflectedLayout {
            std::vector<vk::DescriptorSetLayoutBinding> bindings;
            std::vector<vk::PushConstantRange> pushRanges;
            uint32_t maxSet = 0;
        };

        ReflectedLayout ReflectSPIRV(const std::vector<uint32_t>& spirv, vk::ShaderStageFlagBits stageBits);

        void MergeReflectedLayouts(ReflectedLayout& dst, const ReflectedLayout& src);

        vk::PipelineLayout BuildPipelineLayout(Core::Vulkan::VulkanContext& context, const ReflectedLayout& layout,
            std::vector<vk::DescriptorSetLayout>& outSetLayouts);


        void DestroyPipeline(Core::Vulkan::VulkanContext context, Pipeline& pipeline);
	};

    Pipeline CreateComputePipeline(Core::Vulkan::VulkanContext & context, const ComputePipelineConfig& computePipeConfig);

    Pipeline CreateGraphicsPipeline(Core::Vulkan::VulkanContext & context, const GraphicsPipelineConfig& graphicsPipeConfig);


    struct ShaderModule {
        vk::ShaderModule handle;
        void LoadSPIRV(const Core::Vulkan::VulkanContext& context, const std::string& filename);
        void Destroy(Core::Vulkan::VulkanContext context);
    };



}