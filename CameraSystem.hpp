#pragma once


#include "Camera.hpp"
#include <glm/ext.hpp>

class CameraSystem
{
private:
	Camera camera;
	glm::vec3 baseRotation;
	glm::vec3 basePosition;
	glm::vec3 position;
	glm::vec3 rotation;

	void refreshCamera();

public:
	CameraSystem(const glm::vec3 &baseRotation, const glm::vec3 &basePosition, GLfloat width = 1920, GLfloat height = 1080,
	             GLfloat nearPlane = 1,
	             GLfloat farPlane = 50, GLfloat fieldOfView = PI_4);

	void moveForward(float distance);

	void moveBackward(float distance);

	void moveLeft(float distance);

	void moveRight(float distance);

	void moveAlong(glm::vec3 vec, float distance);

	void rotate(glm::vec3 delta);

	glm::mat4 getTransformationMatrix() const;

	Camera &getCamera();

	const glm::vec3 &getBaseRotation() const;

	void setBaseRotation(const glm::vec3 &baseRotation);

	const glm::vec3 &getBasePosition() const;

	void setBasePosition(const glm::vec3 &basePosition);

	const glm::vec3 &getPosition() const;

	void setPosition(const glm::vec3 &position);

	const glm::vec3 &getRotation() const;

	void setRotation(const glm::vec3 &rotation);
};

