add_executable(OceanApp
    main.cpp
 "AppConfig.h")

target_link_libraries(OceanApp PRIVATE
    Core
    Renderer
    Engine
)