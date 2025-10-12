# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/moham/Documents/GitHub/Fluid_Simulator/external/VkFFT/glslang-main")
  file(MAKE_DIRECTORY "C:/Users/moham/Documents/GitHub/Fluid_Simulator/external/VkFFT/glslang-main")
endif()
file(MAKE_DIRECTORY
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-build"
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix"
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/tmp"
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/src/glslang-main-populate-stamp"
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/src"
  "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/src/glslang-main-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/src/glslang-main-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/moham/Documents/GitHub/Fluid_Simulator/out/build/x64-Debug/_deps/glslang-main-subbuild/glslang-main-populate-prefix/src/glslang-main-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
