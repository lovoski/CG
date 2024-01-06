cmake_minimum_required(VERSION 3.20)

project(raytracing)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SRC_FILES *.cpp)

include_directories(${CGINCLUDES})

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_compile_options(${PROJECT_NAME} PRIVATE ${CGFLAGS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CGLIBS})