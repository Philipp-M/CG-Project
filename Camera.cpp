#include "Camera.hpp"
#include <cmath>
#include <iostream>
#include <glm/ext.hpp>

Camera::Camera(GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, GLfloat fieldOfView) :
        width(width),
        height(height),
        nearPlane(nearPlane),
        farPlane(farPlane),
        fieldOfView(fieldOfView)
{
//    setUpTransMatrix();
}

void Camera::setUpTransMatrix()
{
    /*transMatrix[0][0] = (GLfloat) (1.0 / (tan(fieldOfView / 2.0) * ((double) height / (double) width)));
    transMatrix[1][1] = (GLfloat) (1.0 / tan(fieldOfView / 2.0));
    transMatrix[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    transMatrix[2][3] = (GLfloat) -1.0;
    transMatrix[3][2] = (GLfloat) (-2.0 * farPlane * nearPlane / (farPlane - nearPlane));*/
}

glm::vec3 Camera::getPosition() const
{
    return position;
}

void Camera::setPosition(glm::vec3 position)
{
    this->position = position;
}
void Camera::move(glm::vec3 delta)
{
    this->position += delta;
}
glm::mat4 Camera::getTransformationMatrix()
{
    glm::mat4 retMat = glm::perspective(fieldOfView, width / height, nearPlane, farPlane);

    retMat *= glm::rotate(rotX, glm::vec3(1, 0, 0)) * glm::rotate(rotY, glm::vec3(0, 1, 0)) *
              glm::rotate(rotZ, glm::vec3(0, 0, 1));
    retMat *= glm::translate(glm::vec3(position.x, position.y, position.z));
    return retMat;
}

glm::vec3 Camera::getRotation() const
{
    return glm::vec3(rotX, rotY, rotZ);
}

void Camera::setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
}
void Camera::rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
    this->rotX += rotX;
    this->rotY += rotY;
    this->rotZ += rotZ;
}
