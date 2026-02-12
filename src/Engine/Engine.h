#pragma once

#include <GLFW/glfw3.h>
#include "Vulkan/config.h"
#include <Renderer.h>


/*
* including the prebuilt header from the lunarg sdk will load
* most functions, but not all.
*
* Functions can also be dynamically loaded, using the call
*
* PFN_vkVoidFunction vkGetInstanceProcAddr(
	VkInstance                                  instance,
	const char*                                 pName);

 or

 PFN_vkVoidFunction vkGetDeviceProcAddr(
	VkDevice                                    device,
	const char*                                 pName);

	We will look at this later, once we've created an instance and device.
*/

namespace Engine {

	class Engine {

	private:

		// Whether to promnt debug messages in functions
		bool debugMode = true;


		// GLFW Window parameters
		int width{ 640 };
		int height{ 480 };
		GLFWwindow* window{ nullptr };

		vk::PhysicalDevice physicalDevice{ nullptr };


		vk::Instance instance{ nullptr };

		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };


		void build_glfw_window();


//		void make_instance();

		void make_debug_messenger();

	public:
		Engine(Renderer::Renderer renderer);

		~Engine();

	};
}