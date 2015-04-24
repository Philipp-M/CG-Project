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
public:


private:
    GLfloat width;
    GLfloat height;
    GLfloat nearPlane;
    GLfloat farPlane;
    glm::mat4 transMat;
    glm::vec3 position;
    GLfloat rotX;
    GLfloat rotY;
    GLfloat rotZ;

    void setUpTransMatrix();

public:
    Camera(GLfloat width = 1920, GLfloat height = 1080, GLfloat nearPlane = 1, GLfloat farPlane = 50, GLfloat fieldOfView = PI_4);

    void move(glm::vec3 delta);

    void rotate(glm::vec3 delta);

    void scale(glm::vec3 factor);

    void scale(GLfloat factor);

    void resetTransformationMatrix();

    const glm::mat4 &getTransformationMatrix();

    GLfloat getWidth() const;

    void setWidth(GLfloat width);

    GLfloat getHeight() const;

    void setHeight(GLfloat height);
};
