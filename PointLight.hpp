#pragma once


#include <glm/glm.hpp>
#include <GL/gl.h>
#include "Entity.hpp"
#include "ShaderProgram.hpp"

class PointLight
{
private:
	glm::vec3 position;
	glm::vec3 color;
	GLfloat intensity;
	GLfloat attenuation;

public:

	PointLight(const glm::vec3 &position = glm::vec3(0.0), const glm::vec3 &color = glm::vec3(1.0), GLfloat intensity = 1.0f, GLfloat attenuation = 1.0f) :
			position(position), color(color), intensity(intensity), attenuation(attenuation)
	{ }

	const glm::vec3 &getPosition() const;

	void setPosition(const glm::vec3 &position);

	void move(const glm::vec3 &delta);

	const glm::vec3 &getColor() const;

	void setColor(const glm::vec3 &color);

	GLfloat getIntensity() const;

	void setIntensity(GLfloat intensity);

	GLfloat getAttenuation() const;

	void setAttenuation(GLfloat attenuation);

	void insertInShader(const ShaderProgram &shaderProgram, GLuint num);
};

