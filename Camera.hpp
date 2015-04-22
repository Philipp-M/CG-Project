#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include "Entity.hpp"

const GLfloat PI_4 = 0.78539816339744830962;

class Camera : public Entity
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
    Camera(GLfloat width = 1920, GLfloat height = 1080, GLfloat nearPlane = 1, GLfloat farPlane = 50, GLfloat fieldOfView = PI_4);

    void setPosition(glm::vec3 position);

    void move(glm::vec3 delta);

    glm::vec3 getPosition() const;

    glm::vec3 getRotation() const;

    void setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ);

    void rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ);

    glm::mat4 getTransformationMatrix();
};
