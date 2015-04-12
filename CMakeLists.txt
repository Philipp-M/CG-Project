cmake_minimum_required(VERSION 2.8)
project(CG_Project1)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp tiny_obj_loader.cpp tiny_obj_loader.hpp TextureManager.cpp TextureManager.hpp)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(CG_Project1 ${SOURCE_FILES})
target_link_libraries(CG_Project1 ${SDL2_LIBRARY})
target_link_libraries(CG_Project1 ${SDL2_IMAGE_LIBRARY})
