#include "Camera.hpp"
#include <cmath>
#include <iostream>
#include <glm/ext.hpp>

Camera::Camera(GLfloat width, GLfloat height, GLfloat nearPlane, GLfloat farPlane, GLfloat fieldOfView) :
		width(width),
		height(height),
		nearPlane(nearPlane),
		farPlane(farPlane),
		fieldOfView(fieldOfView),
		viewMat(glm::mat4(1.0))
{
}

glm::mat4 Camera::getViewMatrix() const
{
	return viewMat;
}
glm::mat4 Camera::getProjectionMatrix() const
{
	return glm::perspective(fieldOfView, width / height, nearPlane, farPlane);
}

void Camera::scale(glm::vec3 delta)
{
	viewMat *= glm::scale(delta);
}

void Camera::scale(GLfloat factor)
{
	scale(glm::vec3(factor, factor, factor));
}

void Camera::rotate(glm::vec3 delta)
{
	viewMat *= glm::rotate(delta.x, glm::vec3(1, 0, 0)) * glm::rotate(delta.y, glm::vec3(0, 1, 0)) *
	            glm::rotate(delta.z, glm::vec3(0, 0, 1));
}

void Camera::rotateAroundAxis(glm::vec3 a, float w)
{
	a = glm::normalize(a);
	float cosA = cos(w);
	float sinA = sin(w);
	glm::mat4 rot((a.x * a.x * (1 - cosA) + cosA), (a.x * a.y * (1 - cosA) - a.z * sinA), (a.x * a.z * (1 - cosA) + a.y * sinA), 0,
	              (a.x * a.y * (1 - cosA) + a.z * sinA), (a.y * a.y * (1 - cosA) + a.z * cosA), (a.y * a.z * (1 - cosA) - a.x * sinA), 0,
	              (a.x * a.z * (1 - cosA) - a.y * sinA), (a.y * a.z * (1 - cosA) + a.x * sinA), (a.z * a.z * (1 - cosA) + cosA), 0,
	              0, 0, 0, 1);
	viewMat *= rot;
}

void Camera::move(glm::vec3 delta)
{
	viewMat *= glm::translate(delta);
}

void Camera::resetViewMatrix()
{
	viewMat = glm::mat4(1.0);
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

