#pragma once
#include <VulkanCore/VulkanContext.h> 
#include <Logging/Logger.h>
#include "SwapchainResources.h"

namespace TheRenderer::Vulkan
{

    class Swapchain {

	private:
        vk::SurfaceKHR surface = VK_NULL_HANDLE;

        vk::SwapchainKHR swapChainInstance = VK_NULL_HANDLE;

        vk::Format swapChainImageFormat;
        vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        vk::Extent2D extent;
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

        std::vector<vk::ImageView> swapChainImageViews = {};
        std::vector<vk::Image> swapChainImages = {};
        std::vector<FrameBuffer> frameBuffers = {};
        uint32_t imageCount = 0;

        vk::Image depthImage = VK_NULL_HANDLE;
        VmaAllocation depthAlloc = {  };

        vk::ImageView depthImageView = VK_NULL_HANDLE;
        vk::Format depthFormat = vk::Format::eD32Sfloat;

    public:
        void CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, vk::SwapchainKHR oldSwapChain = VK_NULL_HANDLE);
        void RecreateSwapChain(Core::Vulkan::VulkanContext& context, GLFWwindow* window, RenderPass& renderPass);

        vk::SwapchainKHR GetSwapChain() const { return swapChainInstance; }
        const std::vector<vk::Image>& GetImages() const { return swapChainImages; }
        vk::Format GetImageFormat() const { return swapChainImageFormat; }
		vk::Format GetDepthFormat() const { return depthFormat; }
        vk::Extent2D GetExtent() const { return extent; }
        const std::vector<vk::ImageView>& GetImageViews() const { return swapChainImageViews; }
        const std::vector<FrameBuffer>& GetFrameBuffers() const { return frameBuffers; }

       

        void CreateImageViews(const Core::Vulkan::VulkanContext & context);
        void Destroy(const Core::Vulkan::VulkanContext& context);

        void CreateFramebuffers(const Core::Vulkan::VulkanContext& context, RenderPass& renderPass);


		void DestroyImageResources(const Core::Vulkan::VulkanContext& context);

        vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    };
}