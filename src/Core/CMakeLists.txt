add_library(Core STATIC)




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

# Include the directories

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/vkFFT
        ${CMAKE_SOURCE_DIR}/external/VulkanMemoryAllocator/Include
        ${Vulkan_INCLUDE_DIRS}
)

target_sources(Core
    PRIVATE
       
        "Memory/VMAUsage.cpp"
        "Logging/Logger.cpp"

 "Vulkan/VulkanInit.cpp" "Vulkan/VulkanInit.h" "Vulkan/VulkanContext.h" "Config/PhyDeviceFeaturesConfig.h" "Vulkan/PhysicalDeviceSupport.h")

# ---- Link Libraries ----
target_link_libraries(Core PUBLIC
        glfw
        Vulkan::Vulkan
        Vulkan::glslang
        VkFFT
        VulkanMemoryAllocator
)