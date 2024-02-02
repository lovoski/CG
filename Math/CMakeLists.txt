cmake_minimum_required(VERSION 3.20)

project(math)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(${CGDEFINITIONS})

add_executable(VectorOperation VectorOperation.cpp)
target_compile_options(VectorOperation PRIVATE ${CGFLAGS})
target_link_libraries(VectorOperation PRIVATE ${CGLIBS})
