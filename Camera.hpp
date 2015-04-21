#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>
class Camera
{
private:
    GLfloat fieldOfView;
    GLfloat width;
    GLfloat height;
    GLfloat nearPlane;
    GLfloat farPlane;
    glm::vec3 position;
    GLfloat rotX;
    GLfloat rotY;
    GLfloat rotZ;
    void setUpTransMatrix();
public:
    Camera(GLfloat width, GLfloat height, GLfloat clipNear, GLfloat clipFar, GLfloat fieldOfView);
    void setPosition(glm::vec3 position);
    void move(glm::vec3 delta);
    glm::vec3 getPosition();
    glm::vec3 getRot();
    GLfloat getRotX();
    GLfloat getRotY();
    GLfloat getRotZ();
    void setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ);
    void rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ);
    glm::mat4 getTransformationMatrix();
};
