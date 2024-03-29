cmake_minimum_required(VERSION 3.20)

project(CG)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

add_subdirectory(external/raylib)
add_subdirectory(external/eigen3)
add_subdirectory(external/tinyobjloader)

include_directories(
  external/imgui
  external/eigen3
  external/raylib/src
  external/rlimgui
  external/tinyobjloader)

add_library(rlImGui STATIC 
  external/imgui/imgui.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_draw.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_widgets.cpp
  external/rlimgui/rlImGui.cpp)

set(CGLIBS
  raylib
  rlImGui
  tinyobjloader
  Eigen3::Eigen)

set(CGFLAGS
  "")

set(CGDEFINITIONS
  "")

if (OpenMP_CXX_FOUND)
  set(CGDEFINITIONS
    ${CGDEFINITIONS}
    -DOPENMP_FOUND)
  set(CGLIBS
    ${CGLIBS}
    ${OpenMP_CXX_LIBRARIES})
  set(CGFLAGS
    ${CGFLAGS}
    ${OpenMP_CXX_FLAGS})
endif()

if (MSVC)
  add_compile_options("/utf-8")
endif()

add_subdirectory(DS)
add_subdirectory(Math)
add_subdirectory(PhySim)
add_subdirectory(Renderer/Rasterize)
add_subdirectory(Renderer/PathTracing)
add_subdirectory(Tests/UseRayLib)
add_subdirectory(Animation/Kinematics)
add_subdirectory(Animation/Character)