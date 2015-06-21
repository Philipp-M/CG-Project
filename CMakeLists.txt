cmake_minimum_required(VERSION 2.8)
project(CG-Project)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")-DNULL=nullptr
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNULL=nullptr")

# find_package(OpenMP)
# if (OPENMP_FOUND)
#     set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#     set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
# endif()
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
#########################################################
# FIND GLEW
#########################################################
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()
##########################################################
## FIND Image libraries
##########################################################
#find_package(PNG REQUIRED)
#include_directories(${PNG_INCLUDE_DIR})
#link_libraries(${PNG_LIBRARY})
#find_package(JPEG REQUIRED)
#include_directories(${JPEG_INCLUDE_DIR})
#link_libraries(${JPEG_LIBRARY})


#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
find_package(GLM REQUIRED)
set(SOIL
			src/SOIL/SOIL.c
			src/SOIL/SOIL.h
			src/SOIL/image_DXT.c
			src/SOIL/image_DXT.h
			src/SOIL/image_helper.c
			src/SOIL/image_helper.h
			src/SOIL/stb_image_aug.c
			src/SOIL/stb_image_aug.h
			src/SOIL/stbi_DDS_aug.h
			src/SOIL/stbi_DDS_aug_c.h)

set(SOURCE_FILES
                src/main.cpp
                src/tiny_obj_loader.cpp
                src/tiny_obj_loader.hpp
                src/TextureManager.hpp
                src/MaterialManager.hpp
                src/picojson.hpp
                src/Scene.cpp
                src/Scene.hpp
                src/Camera.cpp
                src/Camera.hpp
                src/Model.cpp
                src/Model.hpp
                src/Shader.cpp
                src/Shader.hpp
                src/ShaderProgram.cpp
                src/ShaderProgram.hpp
                src/ShaderProgramManager.hpp
                src/GlutMainLoop.cpp
                src/GlutMainLoop.hpp
                src/CameraSystem.cpp
                src/CameraSystem.hpp
                src/PointLight.cpp
                src/PointLight.hpp
                src/Color.cpp
                src/Color.hpp)
add_executable(CG-Project ${SOURCE_FILES} ${SOIL} )
target_include_directories(CG-Project PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(CG-Project ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )

