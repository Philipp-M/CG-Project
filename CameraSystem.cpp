#include "CameraSystem.hpp"

CameraSystem::CameraSystem(const glm::vec3 &baseRotation, const glm::vec3 &basePosition, GLfloat width, GLfloat height,
                           GLfloat nearPlane,
                           GLfloat farPlane, GLfloat fieldOfView) : baseRotation(baseRotation), basePosition(basePosition),
                                                                    camera(width, height, nearPlane, farPlane, fieldOfView)
{
	refreshCamera();
}

void CameraSystem::refreshCamera()
{
	camera.resetTransformationMatrix();
	camera.rotate(baseRotation);
	camera.move(basePosition);
}

void CameraSystem::moveForward(float distance)
{
	moveAlong(glm::vec3(0, 0, 1), distance);
}

void CameraSystem::moveBackward(float distance)
{
	moveAlong(glm::vec3(0, 0, -1), distance);
}

void CameraSystem::moveLeft(float distance)
{
	moveAlong(glm::vec3(1, 0, 0), distance);
}

void CameraSystem::moveRight(float distance)
{
	moveAlong(glm::vec3(-1, 0, 0), distance);
}

void CameraSystem::moveAlong(glm::vec3 vec, float distance)
{
	vec = glm::normalize(vec);
	glm::mat4 rot1 = glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) *
	                 glm::rotate(rotation.z, glm::vec3(0, 0, 1));
	glm::mat4 rot2 = glm::rotate(rotation.z, glm::vec3(0, 0, -1)) * glm::rotate(rotation.x, glm::vec3(-1, 0, 0)) *
	                 glm::rotate(rotation.y, glm::vec3(0, -1, 0));
	position = glm::vec3(rot2 * glm::translate(vec * distance) * rot1 * glm::vec4(position, 1));
}

glm::mat4 CameraSystem::getTransformationMatrix() const
{
	glm::mat4 rot = glm::rotate(rotation.x, glm::vec3(1, 0, 0)) * glm::rotate(rotation.y, glm::vec3(0, 1, 0)) *
	                glm::rotate(rotation.z, glm::vec3(0, 0, 1));
	return camera.getPerspectiveMatrix() * camera.getTransformationMatrix() * rot * glm::translate(position);
}

void CameraSystem::rotate(glm::vec3 delta)
{
	rotation += delta;
}

void CameraSystem::rotateAroundAxis(glm::vec3 o, glm::vec3 a, float w)
{
	a = glm::normalize(a);
	glm::vec3 oldPos = position;
	position = glm::vec3(glm::translate(-o) * glm::rotate(w, a) * glm::translate(o) * glm::vec4(position, 1));

	glm::vec3 oPn = glm::normalize(oldPos+o);
	glm::vec3 pn = glm::normalize(position+o);

	if (!((pn.z == 0 && pn.y == 0) || (oPn.z == 0 && oPn.y == 0) || (pn.x == 0 && pn.y == 0) ||
	      (oPn.x == 0 && oPn.y == 0)))
	{
		float phi = atan2(oPn.y, oPn.x) - atan2(pn.y, pn.x);
		float theta = acos(oPn.z) - acos(pn.z);
		rotation += glm::vec3(theta, 0, phi);
	}

}

Camera &CameraSystem::getCamera()
{
	return camera;
}

const glm::vec3 &CameraSystem::getBaseRotation() const
{
	return baseRotation;
}

void CameraSystem::setBaseRotation(const glm::vec3 &baseRotation)
{
	CameraSystem::baseRotation = baseRotation;
}

const glm::vec3 &CameraSystem::getBasePosition() const
{
	return basePosition;
}

void CameraSystem::setBasePosition(const glm::vec3 &basePosition)
{
	CameraSystem::basePosition = basePosition;
}

const glm::vec3 &CameraSystem::getPosition() const
{
	return position;
}

void CameraSystem::setPosition(const glm::vec3 &position)
{
	CameraSystem::position = position;
}

const glm::vec3 &CameraSystem::getRotation() const
{
	return rotation;
}

void CameraSystem::setRotation(const glm::vec3 &rotation)
{
	CameraSystem::rotation = rotation;
}