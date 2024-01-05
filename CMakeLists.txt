cmake_minimum_required(VERSION 3.20)

project(CG)

find_package(raylib CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)
find_package(OpenMP REQUIRED)

set(CGLIBS
  raylib
  Eigen3::Eigen
)

set(CGFLAGS
  "")

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