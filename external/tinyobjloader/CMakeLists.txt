cmake_minimum_required(VERSION 3.20)

project(tinyobjloader)

add_library(${PROJECT_NAME} STATIC tiny_obj_loader.cc)
