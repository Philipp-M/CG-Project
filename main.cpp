#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "picojson.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "ShaderProgramManager.hpp"
#include "GlutMainLoop.hpp"

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glewExperimental = GL_TRUE;
    //glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);
    glutInitWindowSize(1024, 576);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - Project Merry Go Around!");

    /* Initialize GL extension wrangler */
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        exit(EXIT_FAILURE);
    }

    /* Set background (clear) color to dark blue */
    glClearColor(0.0, 0.0, 0.4, 0.0);

    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void onIdle()
{
    GlutMainLoop::get().onIdle();
}

void onDisplay()
{
    GlutMainLoop::get().onDraw();
}

int main(int argc, char *argv[])
{
    init(argc, argv);
    GlutMainLoop::get().init();
    GLuint VAO;
    // some intel hd hack...
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* Specify callback functions;enter GLUT event processing loop,
     * handing control over to GLUT */
    glutIdleFunc(onIdle);
    glutDisplayFunc(onDisplay);
    glutMainLoop();
    return 0;
}