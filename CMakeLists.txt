cmake_minimum_required(VERSION 3.20)

project(CG)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

add_subdirectory(external/raylib)
add_subdirectory(external/eigen3)

set(CGLIBS
  raylib
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

add_subdirectory(Math)
add_subdirectory(RayTracing)
add_subdirectory(UseRayLib)
add_subdirectory(RasterizeRenderer)