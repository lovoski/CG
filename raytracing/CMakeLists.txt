cmake_minimum_required(VERSION 3.20)

project(raytracing)

file(GLOB SRC_FILES *.cpp)

include_directories(
  ${CMAKE_SOURCE_DIR}/common)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CGFLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CGLIBS})