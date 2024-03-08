cmake_minimum_required(VERSION 3.20)

project(useraylib)

set(RESOURCES_PATH ${PROJECT_SOURCE_DIR}/resources)
configure_file(${PROJECT_SOURCE_DIR}/ResourcesConfig.h.in ResourceConfig.h)

include_directories(${CMAKE_SOURCE_DIR}/external/raygui/src
  ${PROJECT_BINARY_DIR})

add_definitions(${CGDEFINITIONS})

add_executable(ChineseFont ChineseFont.cpp)
target_compile_options(ChineseFont PRIVATE ${CGFLAGS})
target_link_libraries(ChineseFont PRIVATE ${CGLIBS})

add_executable(DrawTest DrawTest.cpp)
target_compile_options(DrawTest PRIVATE ${CGFLAGS})
target_link_libraries(DrawTest PRIVATE ${CGLIBS})

add_executable(Interaction Interaction.cpp)
target_compile_options(Interaction PRIVATE ${CGFLAGS})
target_link_libraries(Interaction PRIVATE ${CGLIBS})

add_executable(Trajectory Damper/Trajectory.cpp)
target_compile_options(Trajectory PRIVATE ${CGFLAGS})
target_link_libraries(Trajectory PRIVATE ${CGLIBS})

add_executable(Damper Damper/Damper.cpp)
target_compile_options(Damper PRIVATE ${CGFLAGS})
target_link_libraries(Damper PRIVATE ${CGLIBS})