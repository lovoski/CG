cmake_minimum_required(VERSION 3.20)

project(Rasterize)

file(GLOB RASTERIZE_RENDERER_SRC_FILES *.cpp)

add_definitions(${CGDEFINITIONS})

add_executable(${PROJECT_NAME} ${RASTERIZE_RENDERER_SRC_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CGFLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CGLIBS})
