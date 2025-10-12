cmake_minimum_required(VERSION 3.72)
project(FluidSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")

# ---- Find Packages ----
find_package(Vulkan REQUIRED COMPONENTS glslang)
message ("Found GLSlang at: " ${Vulkan_glslang_INCLUDE_DIRS})
find_package(glfw3 REQUIRED)


# ---- Include Directories ----

# ---- Including Vulkan ----
if (Vulkan_FOUND)
    message(STATUS "Vulkan found at: ${Vulkan_INCLUDE_DIRS}")
    include_directories(${Vulkan_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK environment variable.")
endif()
include_directories("$ENV{VULKAN_SDK}/Include")

# ---- 
include_directories(${CMAKE_SOURCE_DIR}/external/vkFFT/)


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

# Add ImGui (you can add its .cpp files directly)
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)

# ---- Sources ----
add_executable(FluidSimulator
    src/main.cpp
 src/engine.cpp src/engine.h)

# ---- Link Libraries ----
target_link_libraries(FluidSimulator PRIVATE
        glfw
        Vulkan::Vulkan
        VkFFT
)

# ---- Add Vulkan Library Path ----
link_directories("$ENV{VULKAN_SDK}/Lib")
