
            message(WARNING "Using `spirv-remapTargets.cmake` is deprecated: use `find_package(glslang)` to find glslang CMake targets.")

            if (NOT TARGET glslang::spirv-remap)
                include("C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/install/x64-Debug/lib/cmake/glslang/glslang-targets.cmake")
            endif()

            add_library(spirv-remap ALIAS glslang::spirv-remap)
        