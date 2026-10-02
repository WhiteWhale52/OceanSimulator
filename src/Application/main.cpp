#include <TheRenderer.h>
#include <VulkanCore/VulkanInit.h>
#include <Config/AppConfig.h>

static void FramebufferResizeCallback(GLFWwindow* window, int, int) {
	auto* renderer = static_cast<TheRenderer::Renderer*>(glfwGetWindowUserPointer(window));
	if (renderer) {
		renderer-> NotifyResized();
	}
}

int main() {
	
	Core::Logging::Logger* logger = Core::Logging::Logger::get_logger();

	Core::Config::AppConfig appConfig{};
	appConfig.appName = "Ocean Waves";
	appConfig.engineName = "Engine Name";

	Core::Vulkan::VulkanContext context{};

	Core::Vulkan::CreateInstance(context, appConfig);
	GLFWwindow* window = Core::Vulkan::CreateGLFWWindow(context);
	Core::Vulkan::CreateSurface(context, window);
	Core::Vulkan::ChoosePhysicalDevice(context);
	Core::Vulkan::CreateDeviceAndQueues(context);
	Core::Vulkan::VMASetUp(context);
	Core::Vulkan::CreateCommandPools(context);

	{
		TheRenderer::Renderer renderer(context);
		renderer.Init(window);

		glfwSetWindowUserPointer(window, &renderer);
		glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			renderer.DrawFrame();
		}

		renderer.Shutdown();
	}

	Core::Vulkan::Destroy(context);
	return 0;
}
