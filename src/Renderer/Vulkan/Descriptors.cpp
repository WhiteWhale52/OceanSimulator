#include "Descriptors.h"

namespace TheRenderer::Vulkan {
	Descriptors::Descriptors(Core::Vulkan::VulkanContext& context) : context(context){}

	void Descriptors::CreateCameraDescriptorSetLayout()
	{
		vk::DescriptorSetLayoutBinding binding{};
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = vk::DescriptorType::eUniformBuffer;
		binding.stageFlags = vk::ShaderStageFlagBits::eVertex;

		vk::DescriptorSetLayoutCreateInfo layoutinfo{};
		layoutinfo.bindingCount = 1;
		layoutinfo.pBindings = &binding;

		if (context.logicalDevice.createDescriptorSetLayout(&layoutinfo, nullptr, &cameraDescriptorLayout) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create a descriptor set layout for the camera");
		}

	}

	void Descriptors::CreateDescriptorPool(uint32_t framesInFlight)
	{
		vk::DescriptorPoolSize poolSize{};
		poolSize.type = vk::DescriptorType::eUniformBuffer;
		poolSize.descriptorCount = framesInFlight;

		vk::DescriptorPoolCreateInfo createInfo{};
		createInfo.poolSizeCount = 1;
		createInfo.pPoolSizes = &poolSize;
		createInfo.maxSets = framesInFlight;

		if (context.logicalDevice.createDescriptorPool(&createInfo, nullptr, &pool) != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to create a descriptor Pools");
		}
	}

	std::vector<vk::DescriptorSet> Descriptors::AllocateCameraDescriptorSets(uint32_t framesInFlight)
	{
		std::vector<vk::DescriptorSetLayout> layout(framesInFlight, cameraDescriptorLayout);

		vk::DescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.descriptorPool = pool;
		allocateInfo.descriptorSetCount = framesInFlight;
		allocateInfo.pSetLayouts = layout.data();

		return context.logicalDevice.allocateDescriptorSets(allocateInfo);
	}

	
		
}