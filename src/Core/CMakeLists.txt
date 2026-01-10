add_library(Core STATIC)




# ---- Including Vulkan ----
if (Vulkan_FOUND)
    message(STATUS "Vulkan found at: ${Vulkan_INCLUDE_DIRS}")
    target_include_directories(Core PUBLIC ${Vulkan_INCLUDE_DIRS})

else()
    message(FATAL_ERROR "Vulkan SDK not found! Please set VULKAN_SDK environment variable.")
endif()
target_include_directories(Core PUBLIC "$ENV{VULKAN_SDK}/Include")
# ---- Add Vulkan Library Path ----
link_directories("$ENV{VULKAN_SDK}/Lib")




# ---- Include VulkanMemoryAllocator ----
add_library(VulkanMemoryAllocator INTERFACE)
target_include_directories(VulkanMemoryAllocator INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/external/VulkanMemoryAllocator/Include")


# Add ImGui (you can add its .cpp files directly)
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)

# Include the directories

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
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
        VulkanMemoryAllocator
)