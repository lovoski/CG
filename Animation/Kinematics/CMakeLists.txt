cmake_minimum_required(VERSION 3.20)

project(Kinematics)

set(RESOURCE_PATH ${PROJECT_SOURCE_DIR})
configure_file(config.h.in kinematics_config.h)

include_directories(${PROJECT_BINARY_DIR})

add_definitions(${CGDEFINITIONS})

add_executable(FABRIK FABRIK.cpp)
target_compile_options(FABRIK PRIVATE ${CGFLAGS})
target_link_libraries(FABRIK PRIVATE ${CGLIBS})

add_executable(FABRIK-full FABRIK-full.cpp)
target_compile_options(FABRIK-full PRIVATE ${CGFLAGS})
target_link_libraries(FABRIK-full PRIVATE ${CGLIBS})

add_executable(CCDIK CCDIK.cpp)
target_compile_options(CCDIK PRIVATE ${CGFLAGS})
target_link_libraries(CCDIK PRIVATE ${CGLIBS})
