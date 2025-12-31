#include "Engine.h"
#include <Vulkan/VulkanContext.h>
#include <Logging/Logger.h>

namespace Engine {



	Engine::Engine(Renderer::Renderer renderer) {

		if (debugMode) {
			std::cout << "Making a graphics window \n";
		}

	}

	void Engine::build_glfw_window() {

		// initialize glfw
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		if (window = glfwCreateWindow(width, height, "Ocean Waves", nullptr, nullptr)) {
			if (debugMode) {
				std::cout << "Successfully made a GLFW window called \"Ocean Waves \", width: " << width << ", height: " << height << "\n";
			}
		}
		else {
			if (debugMode) {
				std::cout << "GLFW Window creation failed \n";
			}
		}
	}



	void Engine::make_debug_messenger() {
		if (!debugMode) {
			return;
		}
		debugMessenger = Core::Logging::make_debug_messenger(instance, debugMessenger);
	}

	Engine::~Engine() {
		if (debugMode) {
			std::cout << "Goodbye see you \n";
		}

		glfwTerminate();
		Core::Logging::destroy_debug_messenger(instance, debugMessenger);

		instance.destroy();
	}
}