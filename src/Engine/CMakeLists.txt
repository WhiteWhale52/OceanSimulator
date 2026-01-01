add_library(Engine STATIC)

target_include_directories(Engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(Engine PUBLIC
"Engine.cpp"
)

target_link_libraries(Engine PUBLIC
    Core
    Renderer
)