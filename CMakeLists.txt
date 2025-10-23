cmake_minimum_required(VERSION 3.72)
project(FluidSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")

# ---- Find Packages ----
find_package(Vulkan REQUIRED COMPONENTS glslang)
message ("Found GLSlang at: " "${Vulkan_INCLUDE_DIR}/glslang/Include")


# ---- Find GLFW3 on System ----
find_package(glfw3 REQUIRED)


# ---- Including Vulkan ----
if (Vulkan_FOUND)
    message(STATUS "Vulkan found at: ${Vulkan_INCLUDE_DIRS}")
    include_directories(${Vulkan_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK environment variable.")
endif()
include_directories("$ENV{VULKAN_SDK}/Include")
# ---- Add Vulkan Library Path ----
link_directories("$ENV{VULKAN_SDK}/Lib")

# ---- Include vkFFT ---- 
add_library(VkFFT INTERFACE)
target_include_directories(VkFFT INTERFACE
     ${CMAKE_SOURCE_DIR}/external/vkFFT
     ${Vulkan_INCLUDE_DIRS}
     "${Vulkan_INCLUDE_DIR}/glslang/Include"
)
target_link_libraries(VkFFT INTERFACE
    Vulkan::Vulkan
    Vulkan::glslang
)
target_compile_definitions(VkFFT INTERFACE -DVKFFT_BACKEND=0) 


# ---- Include VulkanMemoryAllocator ----
add_library(VulkanMemoryAllocator INTERFACE)
target_include_directories(VulkanMemoryAllocator INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/external/VulkanMemoryAllocator/Include")


# Add ImGui (you can add its .cpp files directly)
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)

# ---- Sources ----
add_executable(FluidSimulator
    "src/main.cpp"
    "src/logger.h"
    "src/logger.cpp"
    "src/VMAUsage.cpp"
    "src/instance.cpp"
    "src/engine.cpp"
    )

# ---- Link Libraries ----
target_link_libraries(FluidSimulator PRIVATE
        glfw
        Vulkan::Vulkan
        VkFFT
        VulkanMemoryAllocator
)

