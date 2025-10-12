#include "engine.h"
#include "instance.h"
#include <iostream>


Engine::Engine() {

	if (debugMode) {
		std::cout << "Making a graphics window \n" ;
	}

	build_glfw_window();

	make_instance();
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


void Engine::make_instance() {
	instance = vkInit::make_instance(debugMode, "Ocean Waves");
}

Engine::~Engine() {
	if (debugMode) {
		std::cout << "Goodbye see you \n";
	}

	glfwTerminate();

	instance.destroy();
}