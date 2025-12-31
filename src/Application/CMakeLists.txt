add_executable(OceanApp
    main.cpp
 "../Core/Config/AppConfig.h")

target_link_libraries(OceanApp PRIVATE
    Core
    Renderer
    Engine
)