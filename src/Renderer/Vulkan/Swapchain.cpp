#include "Swapchain.h"

namespace TheRenderer::Vulkan
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

    void Swapchain::CreateSwapchain(const Core::Vulkan::VulkanContext& context, GLFWwindow* window, vk::SwapchainKHR oldSwapChain)
    {

        vk::SurfaceCapabilitiesKHR capabilities = context.physicalDevice.getSurfaceCapabilitiesKHR(context.surface);

        uint32_t formatCount = 0;
        context.physicalDevice.getSurfaceFormatsKHR(context.surface, &formatCount, nullptr);
        std::vector<vk::SurfaceFormatKHR> surfaceFormats(formatCount);
        context.physicalDevice.getSurfaceFormatsKHR(context.surface, &formatCount, surfaceFormats.data());

        uint32_t modeCount = 0;
        context.physicalDevice.getSurfacePresentModesKHR(context.surface, &modeCount, nullptr);
        std::vector<vk::PresentModeKHR> surfaceModes(modeCount);
        context.physicalDevice.getSurfacePresentModesKHR(context.surface, &modeCount, surfaceModes.data());

        auto t_surfaceFormat = ChooseSwapSurfaceFormat(surfaceFormats);
        swapChainImageFormat = t_surfaceFormat.format;
        colorSpace = t_surfaceFormat.colorSpace;
        presentMode = ChooseSwapPresentMode(surfaceModes);
        extent = ChooseSwapExtent(capabilities, window);

        uint32_t desriedImageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && desriedImageCount > capabilities.maxImageCount)
            desriedImageCount = capabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR info{};
        info.surface = context.surface;
        info.minImageCount = capabilities.minImageCount + 1;
        info.imageFormat = swapChainImageFormat;
        info.imageColorSpace = colorSpace;
        info.imageExtent = extent;
        info.imageArrayLayers = 1;
        info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        info.imageSharingMode = vk::SharingMode::eExclusive;
        info.preTransform = capabilities.currentTransform;
        info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        info.presentMode = presentMode;
        info.clipped = VK_TRUE;
        info.oldSwapchain = oldSwapChain;

        swapChainInstance = context.logicalDevice.createSwapchainKHR(info);

        // Get swapchain images (two-call pattern)
        uint32_t swapchainImageCount = 0;
        context.logicalDevice.getSwapchainImagesKHR(swapChainInstance, &swapchainImageCount, nullptr);
        swapChainImages.resize(swapchainImageCount);
        context.logicalDevice.getSwapchainImagesKHR(swapChainInstance, &swapchainImageCount, swapChainImages.data());
        imageCount = swapchainImageCount;

        CreateImageViews(context);

        // Depth image
        vk::ImageCreateInfo depthInfo{};
        depthInfo.imageType = vk::ImageType::e2D;
        depthInfo.format = depthFormat;
        depthInfo.extent = vk::Extent3D{ extent.width, extent.height, 1 };
        depthInfo.mipLevels = 1;
        depthInfo.arrayLayers = 1;
        depthInfo.samples = vk::SampleCountFlagBits::e1;
        depthInfo.tiling = vk::ImageTiling::eOptimal;
        depthInfo.usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;

        VmaAllocationCreateInfo depthAllocInfo{};
        depthAllocInfo.usage = VMA_MEMORY_USAGE_AUTO;

        if (vmaCreateImage(context.vmaAllocator, reinterpret_cast<const VkImageCreateInfo*>(&depthInfo), &depthAllocInfo,
            reinterpret_cast<VkImage*>(&depthImage), &depthAlloc, nullptr) != VK_SUCCESS)
            throw std::runtime_error("Failed to create depth image");

        vk::ImageViewCreateInfo depthViewInfo{};
        depthViewInfo.image = depthImage;
        depthViewInfo.viewType = vk::ImageViewType::e2D;
        depthViewInfo.format = depthFormat;
        depthViewInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
        depthViewInfo.subresourceRange.levelCount = 1;
        depthViewInfo.subresourceRange.layerCount = 1;
        context.logicalDevice.createImageView(&depthViewInfo, nullptr, &depthImageView);
    }

    void Swapchain::CreateFramebuffers(const Core::Vulkan::VulkanContext& context, RenderPass& renderPass)
    {
        frameBuffers.clear();
        frameBuffers.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImages.size() - 1; i++) {
            frameBuffers[i].Create(context, renderPass, swapChainImageViews[i], depthImageView,
                extent.width, extent.height);
           // frameBuffers.push_back(frameBuffers[i]);
        }
    }


    void Swapchain::RecreateSwapChain(Core::Vulkan::VulkanContext& context, GLFWwindow* window, RenderPass& renderPass)
    {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        while (width == 0 || height == 0) {
            glfwWaitEvents();
            glfwGetFramebufferSize(window, &width, &height);
        }

        context.logicalDevice.waitIdle();

        vk::SwapchainKHR oldSwapchain = swapChainInstance;
        DestroyImageResources(context);
        CreateSwapchain(context, window, oldSwapchain);
        CreateFramebuffers(context, renderPass);

        context.logicalDevice.destroySwapchainKHR(oldSwapchain);

    }


    void Swapchain::CreateImageViews(const Core::Vulkan::VulkanContext& context)
    {
        swapChainImageViews.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            vk::ImageViewCreateInfo viewCreateInfo{};
            viewCreateInfo.image = swapChainImages[i];
            viewCreateInfo.viewType = vk::ImageViewType::e2D;
            viewCreateInfo.format = swapChainImageFormat;

            viewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = 1;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;
            viewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            viewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;

            if (context.logicalDevice.createImageView(&viewCreateInfo, nullptr, &swapChainImageViews[i]) != vk::Result::eSuccess) {
                Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();
                logger->print("Failed to create image view");
            }
        }
    }

    void Swapchain::DestroyImageResources(const Core::Vulkan::VulkanContext& context)
    {
        for (auto& frameBuffer : frameBuffers) {
            frameBuffer.Destroy(context);
        }
        frameBuffers.clear();

        if (depthImageView)
            context.logicalDevice.destroyImageView(depthImageView);
        if (depthImage)
            vmaDestroyImage(context.vmaAllocator, depthImage, depthAlloc);
        depthImageView = VK_NULL_HANDLE;
        depthImage = VK_NULL_HANDLE;

        for (auto imageView : swapChainImageViews)
            context.logicalDevice.destroyImageView(imageView);
        swapChainImageViews.clear();
        swapChainImages.clear();
    }

    void Swapchain::Destroy(const Core::Vulkan::VulkanContext& context)
    {
        DestroyImageResources(context);
        if (swapChainInstance)
            context.logicalDevice.destroySwapchainKHR(swapChainInstance);
        swapChainInstance = VK_NULL_HANDLE;
    }

}