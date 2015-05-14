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
			SOIL/SOIL.c
			SOIL/SOIL.h
			SOIL/image_DXT.c
			SOIL/image_DXT.h
			SOIL/image_helper.c
			SOIL/image_helper.h
			SOIL/stb_image_aug.c
			SOIL/stb_image_aug.h
			SOIL/stbi_DDS_aug.h
			SOIL/stbi_DDS_aug_c.h)

set(SOURCE_FILES
                main.cpp
                tiny_obj_loader.cpp
                tiny_obj_loader.hpp
                TextureManager.hpp
                MaterialManager.hpp
                picojson.hpp
                Scene.cpp
                Scene.hpp
                Camera.cpp
                Camera.hpp
                Model.cpp
                Model.hpp
                Shader.cpp
                Shader.hpp
                ShaderProgram.cpp
                ShaderProgram.hpp
                ShaderProgramManager.hpp
                GlutMainLoop.cpp
                GlutMainLoop.hpp
                CameraSystem.cpp
                CameraSystem.hpp
                PointLight.cpp
                PointLight.hpp)
add_executable(CG-Project ${SOURCE_FILES} ${SOIL})
target_include_directories(CG-Project PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(CG-Project ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )

