#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

const GLfloat PI_4 = 0.78539816339744830962;

class Camera
{
private:
	GLfloat fieldOfView;
	GLfloat width;
	GLfloat height;
	GLfloat nearPlane;
	GLfloat farPlane;
	glm::mat4 viewMat;

public:
	Camera(GLfloat width = 1920, GLfloat height = 1080, GLfloat nearPlane = 1, GLfloat farPlane = 50, GLfloat fieldOfView = PI_4);

	GLfloat getWidth() const;

	void setWidth(GLfloat width);

	GLfloat getHeight() const;

	void setHeight(GLfloat height);

	/*** implemented from the interface Entity ***/

	void move(glm::vec3 delta);

	void rotate(glm::vec3 delta);

	void rotateAroundAxis(glm::vec3 a, float w);

	void scale(glm::vec3 factor);

	void scale(GLfloat factor);

	void resetViewMatrix();

	glm::mat4 getViewMatrix() const;

	glm::mat4 getProjectionMatrix() const;
};
