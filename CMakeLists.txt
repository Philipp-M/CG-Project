cmake_minimum_required(VERSION 2.8)
project(CG-Project)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")-DNULL=nullptr
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNULL=nullptr")
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

#find_package(SDL2 REQUIRED)
#find_package(SDL2_image REQUIRED)
find_package(GLM REQUIRED)

set(SOURCE_FILES
                main.cpp
                tiny_obj_loader.cpp
                tiny_obj_loader.hpp
                #TextureManager.cpp
                #MaterialManager.hpp
                picojson.hpp
                Scene.cpp
                Scene.hpp
                Camera.cpp
                Camera.hpp
                Model.cpp
                Model.hpp
                Entity.hpp
                Shader.cpp
                Shader.hpp
                ShaderProgram.cpp
                ShaderProgram.hpp
                ShaderProgramManager.hpp
                GlutMainLoop.cpp
                GlutMainLoop.hpp)
add_executable(CG-Project ${SOURCE_FILES} )
target_include_directories(CG-Project PUBLIC ${GLM_INCLUDE_DIR})
target_link_libraries(CG-Project ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} )


