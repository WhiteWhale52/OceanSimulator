add_library(Renderer STATIC)

target_include_directories(Renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

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

target_compile_definitions(VkFFT
    INTERFACE
        VKFFT_BACKEND=0
)

target_sources(Renderer PRIVATE
"Renderer.cpp"
"FFT/VkFFTSystem.cpp"
 "Renderer.h" "FrameResources.h" "Vulkan/CommandBuffers.cpp" "Vulkan/Pipelines.cpp" "Vulkan/Descriptors.cpp" "Vulkan/Descriptors.h" "Vulkan/CommandBuffers.h" "Vulkan/Pipelines.h" "Vulkan/Swapchain.h" "Vulkan/Swapchain.cpp")

target_link_libraries(Renderer PUBLIC
    Core
    VkFFT
)