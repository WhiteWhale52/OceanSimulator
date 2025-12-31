add_library(Renderer STATIC)

target_include_directories(Renderer PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(Renderer PUBLIC
"Renderer.cpp"
"FFT/VkFFTSystem.cpp"
 "Renderer.h")

target_link_libraries(Renderer PUBLIC
    Core
    
)