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


find_program(GLSLC glslc HINTS "$ENV{VULKAN_SDK}/Bin" "$ENV{VULKAN_SDK}/bin")
if (NOT GLSLC)
    message(FATAL_ERROR "glslc not found - check that VULKAN_SDK is set")
else()
    message(STATUS "glslc found at ${GLSLC}")
endif()

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/Shaders")
message(STATUS "Shader source directory: ${SHADER_SOURCE_DIR}")
message(STATUS "Shader binary directory: ${SHADER_BINARY_DIR}")
file(MAKE_DIRECTORY "${SHADER_BINARY_DIR}")

file(GLOB SHADER_SOURCES
    ${SHADER_SOURCE_DIR}/*.vert
    ${SHADER_SOURCE_DIR}/*.frag
    ${SHADER_SOURCE_DIR}/*.comp
)


foreach(SHADER ${SHADER_SOURCES})
    get_filename_component(SHADER_NAME "${SHADER}" NAME)
    set(SPIRV_OUTPUT "${SHADER_BINARY_DIR}/${SHADER_NAME}.spv")

    message(STATUS "Compiling shader: ${SHADER_NAME} -> ${SPIRV_OUTPUT}")

    add_custom_command(
        OUTPUT "${SPIRV_OUTPUT}"
        COMMAND "${GLSLC}" "${SHADER}" -o "${SPIRV_OUTPUT}"
        DEPENDS "${SHADER}"
        COMMENT "Compiling shader: ${SHADER_NAME}"
        VERBATIM

    )

    list(APPEND SPIRV_BINARY_FILES "${SPIRV_OUTPUT}")
endforeach()
add_custom_target(CompileShaders
    DEPENDS ${SPIRV_BINARY_FILES}
    )
add_dependencies(Renderer CompileShaders)

target_compile_definitions(VkFFT
    INTERFACE
        VKFFT_BACKEND=0
)

target_sources(Renderer PRIVATE
"TheRenderer.cpp"
"FFT/VkFFTSystem.cpp"
 "TheRenderer.h" 
 "FrameResources.h"
 "Vulkan/CommandBuffers.cpp"
 "Vulkan/Pipelines.cpp"
 "Vulkan/Descriptors.cpp" 
 "Vulkan/Descriptors.h" 
 "Vulkan/CommandBuffers.h"
 "Vulkan/Pipelines.h"
 "Vulkan/Swapchain.h" 
 "Vulkan/Swapchain.cpp"
 "Vulkan/SwapchainResources.cpp"
 "Vulkan/SwapchainResources.h"
  "Vulkan/VulkanResources.h" "Vulkan/VulkanResources.cpp" "Vulkan/Vertex.h" "Vulkan/TriangleSetup.h")

target_link_libraries(Renderer PUBLIC
    Core
    VkFFT
)