#pragma once
#include <vulkan/vulkan.hpp>
#include <Vulkan/VulkanContext.h> 

class Swapchain {
public:
    Swapchain();
    ~Swapchain();

    void Initialize(const Core::Vulkan::VulkanContext& context, vk::SurfaceKHR surface, GLFWwindow* window);
    void Cleanup();
    void Recreate(GLFWwindow* window);

    vk::SwapchainKHR getSwapChain() const { return swapChain; }
    const std::vector<vk::Image>& getImages() const { return swapChainImages; }
    vk::Format getImageFormat() const { return swapChainImageFormat; }
    vk::Extent2D getExtent() const { return swapChainExtent; }
    const std::vector<VkImageView>& getImageViews() const { return swapChainImageViews; }

private:
    vk::SurfaceKHR surface = VK_NULL_HANDLE;

    vk::SwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<vk::Image> swapChainImages;
    vk::Format swapChainImageFormat;
    vk::Extent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;

    void CreateSwapChain(GLFWwindow* window);
    void CreateImageViews();
    void CleanupSwapChain();

    vk::SurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    vk::Extent2D ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
};