#include <cmath>
#include <GL/freeglut.h>
#include "GlutMainLoop.hpp"

GlutMainLoop::GlutMainLoop()
{
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
    camera.setWidth(glutGet(GLUT_WINDOW_WIDTH));
    camera.setHeight(glutGet(GLUT_WINDOW_HEIGHT));
    const std::vector<Model *> &models = scene->getModels();
    for (int i = 0; i < models.size(); i++)
    {
        models[i]->resetTransformationMatrix();
        models[i]->rotate(glm::vec3(0.000, 0.000, -0.0005 * glutGet(GLUT_ELAPSED_TIME)));
        if (models[i]->getName() == "horse1" || models[i]->getName() == "horse3" || models[i]->getName() == "horse5" ||
            models[i]->getName() == "horse7")
        {
            models[i]->move(glm::vec3(0, 0, -0.5 + 0.5 * std::sin(0.001 * glutGet(GLUT_ELAPSED_TIME))));
        }
        if (models[i]->getName() == "horse2" || models[i]->getName() == "horse4" || models[i]->getName() == "horse6" ||
            models[i]->getName() == "horse8")
        {
            models[i]->move(glm::vec3(0, 0, -0.5 + 0.5 * std::sin(0.0015 * glutGet(GLUT_ELAPSED_TIME))));
        }

    }
    glutPostRedisplay();
}

void GlutMainLoop::init()
{
    scene = new Scene("merry.json");
}

GlutMainLoop::~GlutMainLoop()
{
    delete scene;
}
