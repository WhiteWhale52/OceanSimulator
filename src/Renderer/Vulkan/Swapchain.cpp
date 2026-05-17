#include "Swapchain.h"



namespace Renderer::Vulkan
{
    vk::SurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
    {
        for (const auto& format : availableFormats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return format;

            }
        }
        return availableFormats[0];
    }

    vk::PresentModeKHR Swapchain::ChooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes)
    {
        for (const auto& mode : availablePresentModes) {
            if (mode == vk::PresentModeKHR::eMailbox)
                return mode;
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D Swapchain::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
    {
        if (capabilities.currentExtent.width != UINT32_MAX)
            return capabilities.currentExtent;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return {
            std::clamp((uint32_t)width, capabilities.minImageExtent.width,  capabilities.maxImageExtent.width),
            std::clamp((uint32_t)height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
        };
    }

    Swapchain Swapchain::CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, vk::SwapchainKHR oldSwapChain)
    {
        Swapchain t_swapChain{};

        vk::SurfaceCapabilitiesKHR capabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(context.surface);

        // Get format (two-call pattern)
        uint32_t formatCount = 0;
        context.physicalDevice.getSurfaceFormatsKHR(context.surface, &formatCount, nullptr);
        std::vector<vk::SurfaceFormatKHR> surfaceFormats(formatCount);
        context.physicalDevice.getSurfaceFormatsKHR(context.surface, &formatCount, surfaceFormats.data());

        // Get present modes (two-call pattern)
        uint32_t modeCount = 0;
        context.physicalDevice.getSurfacePresentModesKHR(context.surface, &modeCount, nullptr);
        std::vector<vk::PresentModeKHR> surfaceModes(modeCount);
        context.physicalDevice.getSurfacePresentModesKHR(context.surface, &modeCount, surfaceModes.data());

        auto t_surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats);
        t_swapChain.swapChainImageFormat = t_surfaceFormat.format;
        t_swapChain.colorSpace = t_surfaceFormat.colorSpace;
        t_swapChain.presentMode = ChooseSwapPresentMode(surfaceModes);
        t_swapChain.extent = ChooseSwapExtent(capabilities, window);

        vk::SwapchainCreateInfoKHR info{};
        info.surface = context.surface;
        info.minImageCount = capabilities.minImageCount + 1;
        info.imageFormat = t_swapChain.swapChainImageFormat;
        info.imageColorSpace = t_swapChain.colorSpace;
        info.imageExtent = t_swapChain.extent;
        info.imageArrayLayers = 1;
        info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        info.imageSharingMode = vk::SharingMode::eExclusive;
        info.preTransform = capabilities.currentTransform;
        info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        info.presentMode = t_swapChain.presentMode;
        info.clipped = VK_TRUE;
        info.oldSwapchain = oldSwapChain;

        t_swapChain.swapChainInstance = context.logicalDevice.createSwapchainKHR(info);

        // Get swapchain images (two-call pattern)
        uint32_t swapchainImageCount = 0;
        context.logicalDevice.getSwapchainImagesKHR(t_swapChain.swapChainInstance, &swapchainImageCount, nullptr);
        t_swapChain.swapChainImages.resize(swapchainImageCount);
        context.logicalDevice.getSwapchainImagesKHR(t_swapChain.swapChainInstance, &swapchainImageCount, t_swapChain.swapChainImages.data());

        CreateImageViews(context, t_swapChain);

        // Depth image
        vk::ImageCreateInfo depthInfo{};
        depthInfo.imageType = vk::ImageType::e2D;
        depthInfo.format = t_swapChain.depthFormat;
        depthInfo.extent = vk::Extent3D{ t_swapChain.extent.width, t_swapChain.extent.height, 1 };
        depthInfo.mipLevels = 1;
        depthInfo.arrayLayers = 1;
        depthInfo.samples = vk::SampleCountFlagBits::e1;
        depthInfo.tiling = vk::ImageTiling::eOptimal;
        depthInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

        VmaAllocationCreateInfo depthAllocInfo{};
        depthAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        if (vmaCreateImage(context.vmaAllocator, reinterpret_cast<const VkImageCreateInfo*>(&depthInfo), &depthAllocInfo,
            reinterpret_cast<VkImage*>(&t_swapChain.depthImage), &t_swapChain.depthAlloc, nullptr) != VK_SUCCESS)
            throw std::runtime_error("Failed to create depth image");

        vk::ImageViewCreateInfo depthViewInfo{};
        depthViewInfo.image = t_swapChain.depthImage;
        depthViewInfo.viewType = vk::ImageViewType::e2D;
        depthViewInfo.format = t_swapChain.depthFormat;
        depthViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        depthViewInfo.subresourceRange.levelCount = 1;
        depthViewInfo.subresourceRange.layerCount = 1;
        context.logicalDevice.createImageView(&depthViewInfo, nullptr, &t_swapChain.depthImageView);

        return t_swapChain;
    }


    void Renderer::Vulkan::Swapchain::Recreate(Core::Vulkan::VulkanContext& context, GLFWwindow* window, Swapchain& swapChain)
    {
        int width = 0, height = 0;

        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }

        context.logicalDevice.waitIdle();

        vk::SwapchainKHR oldSwapchain = swapChain.swapChainInstance;
        context.logicalDevice.destroyImageView(swapChain.depthImageView);
        vmaDestroyImage(context.vmaAllocator, swapChain.depthImage, swapChain.depthAlloc);

        for (auto imageView : swapChain.swapChainImageViews)
            context.logicalDevice.destroyImageView(imageView);
        swapChain.swapChainImageViews.clear();
        swapChain.swapChainImages.clear();
        swapChain.swapChainInstance = VK_NULL_HANDLE;

        // Create new swapchain, passing old handle so driver can recycle
        swapChain = CreateSwapchain(context, window, oldSwapchain);

        // Now safe to destroy old swapchain
        context.logicalDevice.destroySwapchainKHR(oldSwapchain, nullptr);

    }


    void Renderer::Vulkan::Swapchain::CreateImageViews(const Core::Vulkan::VulkanContext& context, Swapchain& swapChain)
    {
        swapChain.swapChainImageViews.resize(swapChain.swapChainImages.size());

        for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
            vk::ImageViewCreateInfo viewCreateInfo{};
            viewCreateInfo.image = swapChain.swapChainImages[i];
            viewCreateInfo.viewType = vk::ImageViewType::e2D;
            viewCreateInfo.format = swapChain.swapChainImageFormat;

            viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = 1;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;
            viewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

            if (context.logicalDevice.createImageView(&viewCreateInfo, nullptr, &swapChain.swapChainImageViews[i]) != vk::Result::eSuccess) {
                Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
                logger->print("Failed to create image view");
            }
        }
    }

    void Renderer::Vulkan::Swapchain::DestroySwapChain(const Core::Vulkan::VulkanContext& context, Swapchain& swapChain)
    {
        context.logicalDevice.destroyImageView(swapChain.depthImageView);
        vmaDestroyImage(context.vmaAllocator, swapChain.depthImage, swapChain.depthAlloc);

        for (auto imageView : swapChain.swapChainImageViews) 
            context.logicalDevice.destroyImageView(imageView);

            context.logicalDevice.destroySwapchainKHR(swapChain.swapChainInstance);
            swapChain = {};

    }

    
}
