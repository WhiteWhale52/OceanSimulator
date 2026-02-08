#include "Swapchain.h"

Swapchain::Swapchain()
{
}

Swapchain::~Swapchain()
{
}

void Swapchain::Initialize(const Core::Vulkan::VulkanContext& context, vk::SurfaceKHR surface, GLFWwindow* window)
{
}

void Swapchain::Cleanup()
{
}

void Swapchain::Recreate(GLFWwindow* window)
{
}

void Swapchain::CreateSwapChain(GLFWwindow* window)
{
}

void Swapchain::CreateImageViews()
{
}

void Swapchain::CleanupSwapChain()
{
}

vk::SurfaceFormatKHR Swapchain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	return vk::SurfaceFormatKHR();
}

vk::PresentModeKHR Swapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	return vk::PresentModeKHR();
}

vk::Extent2D Swapchain::ChooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
	return vk::Extent2D();
}
