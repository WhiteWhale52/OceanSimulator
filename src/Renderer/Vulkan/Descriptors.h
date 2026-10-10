#pragma once

#include <VulkanCore/VulkanContext.h>

namespace TheRenderer::Vulkan {
	class Descriptors {
	public:
		Descriptors(Core::Vulkan::VulkanContext& context);

		void CreateCameraDescriptorSetLayout();
		void CreateDescriptorPool(uint32_t framesInFlight);
		std::vector<vk::DescriptorSet> AllocateCameraDescriptorSets(uint32_t framesInFlight);

		vk::DescriptorSetLayout cameraDescriptorLayout;
		vk::DescriptorPool pool;
	private:
		Core::Vulkan::VulkanContext& context;

	};
}

