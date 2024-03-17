cmake_minimum_required(VERSION 3.20)

project(DS)

set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/resources)
configure_file(config.h.in ds_config.h)

include_directories(${PROJECT_BINARY_DIR})

add_executable(test_halfedge TestHalfedge.cpp)