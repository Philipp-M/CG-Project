#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>
class Camera
{
private:
    GLfloat fieldOfView;
    GLfloat width;
    GLfloat height;
    GLfloat clipNear;
    GLfloat clipFar;
    glm::mat4x4 transMatrix;
    void setUpTransMatrix();
public:
    Camera(GLfloat width, GLfloat height, GLfloat clipNear, GLfloat clipFar, GLfloat fieldOfView);
};
