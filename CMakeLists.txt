cmake_minimum_required(VERSION 3.72)
project(OceanSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ---- Find Packages ----
find_package(Vulkan REQUIRED COMPONENTS glslang)
message ("Found GLSlang at: " "${Vulkan_INCLUDE_DIR}/glslang/Include")


# ---- Find GLFW3 on System ----
find_package(glfw3 REQUIRED)

add_subdirectory("src/Core")
add_subdirectory("src/Renderer")
add_subdirectory("src/Engine")
add_subdirectory("src/Application")





