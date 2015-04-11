cmake_minimum_required(VERSION 2.8)
project(CG_Project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp tiny_obj_loader.cpp)
add_executable(CG_Project1 ${SOURCE_FILES})