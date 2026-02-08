#pragma once
#include <Vulkan/VulkanContext.h> 
#include <Logging/Logger.h>

namespace Renderer::Vulkan
{

    struct Swapchains {


        vk::SwapchainKHR swapChainInstance = VK_NULL_HANDLE;
        vk::Extent2D swapChainExtent;
        vk::SurfaceKHR surface = VK_NULL_HANDLE;
        std::vector<vk::ImageView> swapChainImageViews;
        std::vector<vk::Image> swapChainImages;
        vk::Format swapChainImageFormat;


        Swapchains CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, uint32_t width, uint32_t height);
        void Cleanup();
        void Recreate(GLFWwindow* window);

        vk::SwapchainKHR GetSwapChain() const { return swapChainInstance; }
        const std::vector<vk::Image>& GetImages() const { return swapChainImages; }
        vk::Format GetImageFormat() const { return swapChainImageFormat; }
        vk::Extent2D GetExtent() const { return swapChainExtent; }
        const std::vector<vk::ImageView>& GetImageViews() const { return swapChainImageViews; }



        void CreateImageViews(Core::Vulkan::VulkanContext& context, Swapchains& swapchain);
        void CleanupSwapChain();

        vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
    };
}