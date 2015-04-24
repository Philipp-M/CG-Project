#include <cmath>
#include <cstddef>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

GlutMainLoop::GlutMainLoop()
{
    for (int i = 0; i < 20; i++)
        horseMDown[i] = false;
}

void GlutMainLoop::onDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();
    glutSwapBuffers();
}

void GlutMainLoop::onIdle()
{
    Camera &camera = scene->getCamera();
    camera.setPosition(glm::vec3(0, -2, -10));
    camera.setRotation(M_PI / 2.0, 0, 0);
    const std::vector<Model *> &models = scene->getModels();
    for (int i = 0; i < models.size(); i++)
    {
        models[i]->rotate(0.000, 0.000, -0.006);
        if (models[i]->getName() == "horse1" || models[i]->getName() == "horse3" || models[i]->getName() == "horse5" ||
            models[i]->getName() == "horse7")
        {
            if (horseMDown[i])
                models[i]->move(glm::vec3(0, 0, -0.01));
            else
                models[i]->move(glm::vec3(0, 0, 0.01));
            if (models[i]->getPosition().z > 0.0)
                horseMDown[i] = true;

            if (models[i]->getPosition().z < -1.0)
                horseMDown[i] = false;
        }
        if (models[i]->getName() == "horse2" || models[i]->getName() == "horse4" || models[i]->getName() == "horse6" ||
            models[i]->getName() == "horse8")
        {
            if (horseMDown[i])
                models[i]->move(glm::vec3(0, 0, -0.03));
            else
                models[i]->move(glm::vec3(0, 0, 0.03));
            if (models[i]->getPosition().z > 0.0)
                horseMDown[i] = true;

            if (models[i]->getPosition().z < -1.0)
                horseMDown[i] = false;
        }

    }
    glutPostRedisplay();
}

void GlutMainLoop::init()
{
    std::vector<Model::Vertex3> vData;
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
    vData.push_back(Model::Vertex3(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));


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

    std::vector<GLuint> iData(index_buffer_data, index_buffer_data + sizeof(index_buffer_data) / sizeof(index_buffer_data[0]));


    //scene = new Scene("");
    scene = Scene::loadFromObj("merry.obj");
    //scene->addModel(new Model("cube",vData, iData));
}

GlutMainLoop::~GlutMainLoop()
{
    delete scene;
}
