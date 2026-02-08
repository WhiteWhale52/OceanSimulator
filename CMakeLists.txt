cmake_minimum_required(VERSION 3.72)
project(OceanSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Vulkan_ROOT "$ENV{VULKAN_SDK}")

# ---- Find Packages ----
find_package(Vulkan REQUIRED COMPONENTS glslang)


message ("Found GLSlang at: " "$ENV{VULKAN_SDK}/Include/glslang/Include")

include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)


add_subdirectory("src/Core")
target_compile_definitions(Core
    PRIVATE
        DEBUG_VULKAN=1
)
add_subdirectory("src/Renderer")
add_subdirectory("src/Engine")
add_subdirectory("src/Application")




