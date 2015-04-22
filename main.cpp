#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <unistd.h>
#include "Camera.hpp"
#include "Model.hpp"
#include "ShaderProgramManager.hpp"
#include "GlutMainLoop.hpp"

void init(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitContextVersion(3, 3);
    //glutInitContextProfile ( GLUT_COMPATIBILITY_PROFILE );
    glutInitContextProfile(GLUT_CORE_PROFILE);

    //glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(400, 400);
    glutCreateWindow("CG Proseminar - Project Merry Go Around!");
    printf("%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    /* Initialize GL extension wrangler */
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
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
    /* Specify callback functions;enter GLUT event processing loop,
     * handing control over to GLUT */
    glutIdleFunc(onIdle);
    glutDisplayFunc(onDisplay);
    glutMainLoop();
    return 0;
}