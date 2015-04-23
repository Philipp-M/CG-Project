#include <cstddef>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

void GlutMainLoop::onDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();
    glutSwapBuffers();
}

void GlutMainLoop::onIdle()
{
    Camera& camera = scene->getCamera();
    camera.setPosition(glm::vec3(0,0,-5));
    Model* cube = scene->getModel("cube");
    if(cube != NULL)
    {
        cube->rotate(0.001,0.0033,0.002);
        cube->move(glm::vec3(0.001,0.0033,0.002));
    }
    glutPostRedisplay();
}

void GlutMainLoop::init()
{
    std::vector<Model::Vertex3> vData;
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f,0.0f,1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f,0.0f,1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f,1.0f,1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f,1.0f,1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f,0.0f,0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f,0.0f,0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f,1.0f,0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f,1.0f,0.0f)));


    static const GLuint index_buffer_data[] = { /* Indices of 6*2 triangles (6 sides) */
            0, 1, 2,
            2, 3, 0,
            1, 5, 6,
            6, 2, 1,
            7, 6, 5,
            5, 4, 7,
            4, 0, 3,
            3, 7, 4,
            4, 5, 1,
            1, 0, 4,
            3, 2, 6,
            6, 7, 3,
    };

    std::vector<GLuint> iData(index_buffer_data, index_buffer_data + sizeof(index_buffer_data) / sizeof(index_buffer_data[0]) );


    scene = new Scene("");
    scene->addModel(new Model("cube",vData, iData)); }

GlutMainLoop::~GlutMainLoop()
{
    delete scene;
}
