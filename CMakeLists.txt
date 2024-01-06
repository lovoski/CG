cmake_minimum_required(VERSION 3.20)

project(CG)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)

add_subdirectory(external/raylib)
add_subdirectory(external/eigen3)

set(CGLIBS
  raylib
  Eigen3::Eigen)

set(CGFLAGS
  "")

set(CGINCLUDES
  ${CMAKE_SOURCE_DIR}/common
  ${CMAKE_SOURCE_DIR}/external/raygui/src)

if (OpenMP_CXX_FOUND)
  add_definitions(-DOPENMP_FOUND)
  set(CGLIBS
    ${CGLIBS}
    ${OpenMP_CXX_LIBRARIES})
  set(CGFLAGS
    ${CGFLAGS}
    ${OpenMP_CXX_FLAGS})
endif()

add_subdirectory(raytracing)