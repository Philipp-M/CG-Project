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

const glm::mat4& Camera::getTransformationMatrix()
{
    return glm::perspective(fieldOfView,width/height,nearPlane,farPlane)*transMat;
}

void Camera::scale(glm::vec3 delta)
{
    transMat *= glm::scale(delta);
}

void Camera::scale(GLfloat factor)
{
    scale(glm::vec3(factor,factor,factor));
}
void Camera::rotate(glm::vec3 delta)
{
    transMat *= glm::rotate(delta.x, glm::vec3(1, 0, 0)) * glm::rotate(delta.y, glm::vec3(0, 1, 0)) *
                glm::rotate(delta.z, glm::vec3(0, 0, 1));
}

void Camera::move(glm::vec3 delta)
{
    transMat *= glm::translate(delta);
}

void Camera::resetTransformationMatrix()
{
    transMat = glm::mat4(1.0);
}

GLfloat Camera::getWidth() const
{
    return width;
}

void Camera::setWidth(GLfloat width)
{
    Camera::width = width;
}

GLfloat Camera::getHeight() const
{
    return height;
}

void Camera::setHeight(GLfloat height)
{
    Camera::height = height;
}

