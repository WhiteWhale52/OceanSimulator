#include "Swapchain.h"





namespace Renderer::Vulkan
{
    Swapchains Swapchains::CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, uint32_t width, uint32_t height)
    {
        Swapchains t_swapChain{};

        vk::SurfaceCapabilitiesKHR caps;
        context.physicalDevice.getSurfaceCapabilitiesKHR(context.surface, &caps);

        t_swapChain.swapChainExtent = vk::Extent2D{ width, height };

        vk::SwapchainCreateInfoKHR info{};
        info.surface = context.surface;
        info.minImageCount = caps.minImageCount + 1;
        info.imageFormat = vk::Format::eB8G8R8A8Srgb;
        info.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        info.imageExtent = t_swapChain.swapChainExtent;
        info.imageArrayLayers = 1;
        info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        info.imageSharingMode = vk::SharingMode::eExclusive;
        info.preTransform = caps.currentTransform;
        info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        info.presentMode = vk::PresentModeKHR::eFifo;
        info.clipped = VK_TRUE;

        context.logicalDevice.createSwapchainKHR(&info, nullptr, &t_swapChain.swapChainInstance);

        uint32_t count;
        context.logicalDevice.getSwapchainImagesKHR(t_swapChain.swapChainInstance, &count, nullptr);
        t_swapChain.swapChainImages.resize(count);
        context.logicalDevice.getSwapchainImagesKHR(t_swapChain.swapChainInstance, &count, t_swapChain.swapChainImages.data());

        t_swapChain.swapChainImageFormat = info.imageFormat;
        return t_swapChain;
    }

    void Swapchains::Cleanup()
    {
    }

    void Swapchains::Recreate(GLFWwindow* window)
    {
    }


    void Swapchains::CreateImageViews(Core::Vulkan::VulkanContext& context, Swapchains& t_swapChain)
    {
        t_swapChain.swapChainImageViews.resize(t_swapChain.swapChainImages.size());

        for (size_t i = 0; i < t_swapChain.swapChainImages.size(); i++) {
            vk::ImageViewCreateInfo viewCreateInfo{};
            viewCreateInfo.image = t_swapChain.swapChainImages[i];
            viewCreateInfo.viewType = vk::ImageViewType::e2D;
            viewCreateInfo.format = t_swapChain.swapChainImageFormat;

            viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = 1;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;
            viewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

            if (context.logicalDevice.createImageView(&viewCreateInfo, nullptr, &t_swapChain.swapChainImageViews[i]) != vk::Result::eSuccess) {
                Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
                logger->print("Failed to create image view");
            }
        }
    }

    void Swapchains::CleanupSwapChain()
    {
    }

    vk::SurfaceFormatKHR Swapchains::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
    {
        for (const auto& format : availableFormats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return format;

            }
        }
        return availableFormats[0];
    }

    vk::PresentModeKHR Swapchains::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
    {
        for (const auto& mode : availablePresentModes) {
            if (mode == vk::PresentModeKHR::eMailbox)
                return mode;
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D Swapchains::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
    {
        return vk::Extent2D();
    }
}
