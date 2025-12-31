add_executable(OceanApp
    main.cpp
)

target_link_libraries(OceanApp PRIVATE
    Core
    Renderer
    Engine
)