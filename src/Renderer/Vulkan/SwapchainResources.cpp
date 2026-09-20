#include "SwapchainResources.h"

namespace TheRenderer::Vulkan {

    void RenderPass::Create(const Core::Vulkan::VulkanContext& context, vk::Format colorFormat, vk::Format depthFormat)
    {

        vk::AttachmentDescription colorAttachement;
        colorAttachement.format = colorFormat;
        colorAttachement.samples = vk::SampleCountFlagBits::e1;
        colorAttachement.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachement.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachement.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachement.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachement.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachement.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        vk::AttachmentDescription depthAttachement;
        depthAttachement.format = depthFormat;
        depthAttachement.samples = vk::SampleCountFlagBits::e1;
        depthAttachement.loadOp = vk::AttachmentLoadOp::eClear;
        depthAttachement.storeOp = vk::AttachmentStoreOp::eStore;
        depthAttachement.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depthAttachement.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        depthAttachement.initialLayout = vk::ImageLayout::eUndefined;
        depthAttachement.finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentReference colorRef;
        colorRef.attachment = 0;
        colorRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depthRef;
        depthRef.attachment = 1;
        depthRef.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentDescription attachments[] = { colorAttachement, depthAttachement };

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pDepthStencilAttachment = &depthRef;
        subpass.pColorAttachments = &colorRef;

        vk::SubpassDependency dependency;
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
        dependency.srcAccessMask = {};
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite;


        vk::RenderPassCreateInfo createInfo;
        createInfo.attachmentCount = 2;
        createInfo.pAttachments = attachments;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;
        createInfo.dependencyCount = 1;
        createInfo.pDependencies = &dependency;

        context.logicalDevice.createRenderPass(&createInfo, nullptr, &handle);
    }

    void RenderPass::Destroy(const Core::Vulkan::VulkanContext& context) {
        if (handle != VK_NULL_HANDLE) {
            context.logicalDevice.destroyRenderPass(handle);
        }
    }

    void FrameBuffer::Create(const Core::Vulkan::VulkanContext& context, RenderPass& renderPass, vk::ImageView colorView, vk::ImageView depthView, uint32_t width, uint32_t height)
    {
        

        vk::ImageView attachments[]{ colorView, depthView };

        vk::FramebufferCreateInfo createInfo;
        createInfo.height = height;
        createInfo.width = width;
        createInfo.renderPass = renderPass.handle;
        createInfo.attachmentCount = 2;
        createInfo.pAttachments = attachments;
        createInfo.layers = 1;
        context.logicalDevice.createFramebuffer(&createInfo, nullptr, &handle);

        return ;
    }

    void FrameBuffer::Destroy(const Core::Vulkan::VulkanContext& context)
    {
        if (handle != VK_NULL_HANDLE) {
            context.logicalDevice.destroyFramebuffer(handle);
        }
    }


   

    void SwapchainResources::Create(const Core::Vulkan::VulkanContext& context, vk::Format colourFormat, vk::Format depthFormat, const std::vector<vk::ImageView>& colourViews, vk::ImageView depthView, uint32_t width, uint32_t height)
    {
        renderPass.Create(context, colourFormat, depthFormat);
        for (uint32_t i; i < colourViews.size(); i++) {
            framebuffers[i].Create(context, renderPass, colourViews[i], depthView, width, height);
        }
    }

    void SwapchainResources::Destroy(const Core::Vulkan::VulkanContext& context)
    {
    }

}