cmake_minimum_required(VERSION 3.20)

project(Kinematics)

add_definitions(${CGDEFINITIONS})

add_executable(FABRIK FABRIK.cpp)
target_compile_options(FABRIK PRIVATE ${CGFLAGS})
target_link_libraries(FABRIK PRIVATE ${CGLIBS})
