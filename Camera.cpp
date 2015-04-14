#include "Camera.hpp"
#include <cmath>
#include <iostream>

Camera::Camera(GLfloat width, GLfloat height, GLfloat clipNear, GLfloat clipFar, GLfloat fieldOfView) :
        width(width),
        height(height),
        clipNear(clipNear),
        clipFar(clipFar),
        fieldOfView(fieldOfView),
        transMatrix(glm::mat4x4(0.0))
{
    setUpTransMatrix();
}

void Camera::setUpTransMatrix()
{
    transMatrix[0][0] = 1.0 / (tan(fieldOfView / 2.0) * ((double) height / (double) width));
    transMatrix[1][1] = 1.0 / tan(fieldOfView / 2.0);
    transMatrix[2][2] = -(clipFar + clipNear) / (clipFar - clipNear);
    transMatrix[2][3] = -1.0;
    transMatrix[3][2] = -2.0 * clipFar * clipNear / (clipFar - clipNear);
}
