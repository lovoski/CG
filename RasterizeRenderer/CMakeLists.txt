cmake_minimum_required(VERSION 3.20)

project(renderer)

file(GLOB RASTERIZE_RENDERER_SRC_FILES *.cpp)

add_definitions(${CGDEFINITIONS})

include_directories(
  ${CMAKE_SOURCE_DIR}/common
  ${CMAKE_SOURCE_DIR}/external/raygui/src)

add_executable(${PROJECT_NAME} ${RASTERIZE_RENDERER_SRC_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CGFLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CGLIBS})
