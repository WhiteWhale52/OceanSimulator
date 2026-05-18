#pragma once
#include <VulkanCore/VulkanContext.h> 
#include <Logging/Logger.h>
#include "SwapchainResources.h"

namespace Renderer::Vulkan
{

    struct Swapchain {


        vk::SurfaceKHR surface = VK_NULL_HANDLE;

        vk::SwapchainKHR swapChainInstance = VK_NULL_HANDLE;

        vk::Format swapChainImageFormat;
        vk::ColorSpaceKHR colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        vk::Extent2D extent;
        vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

        std::vector<vk::ImageView> swapChainImageViews;
        std::vector<vk::Image> swapChainImages;
        std::vector<FrameBuffer> frameBuffers;
        uint32_t imageCount = 0;

        vk::Image depthImage = VK_NULL_HANDLE;
        VmaAllocation depthAlloc = {  };

        vk::ImageView depthImageView = VK_NULL_HANDLE;
        vk::Format depthFormat = vk::Format::eD32Sfloat;


        Swapchain CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, RenderPass& renderPass, vk::SwapchainKHR oldSwapChain);
        void Recreate(Core::Vulkan::VulkanContext& context, GLFWwindow* window, RenderPass& renderPass, Swapchain& swapChain);

        vk::SwapchainKHR GetSwapChain() const { return swapChainInstance; }
        const std::vector<vk::Image>& GetImages() const { return swapChainImages; }
        vk::Format GetImageFormat() const { return swapChainImageFormat; }
        vk::Extent2D GetExtent() const { return extent; }
        const std::vector<vk::ImageView>& GetImageViews() const { return swapChainImageViews; }

        void CreateImageViews(const Core::Vulkan::VulkanContext & context, Swapchain& swapchain);
        void DestroySwapChain(const Core::Vulkan::VulkanContext& context, Swapchain& swapChain);

        vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    };
}