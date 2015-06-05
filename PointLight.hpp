#pragma once


#include <glm/glm.hpp>
#include <GL/gl.h>
#include "ShaderProgram.hpp"
#include "Color.hpp"

class PointLight
{
private:
	glm::vec3 position;
	Color color;
	GLfloat ambient;
	GLfloat attenuation;

public:

	PointLight(const glm::vec3 &position = glm::vec3(0.0), const Color &color = Color::fromRGB(glm::vec3(1)), GLfloat attenuation = 1.0f, GLfloat ambient = 0.1f) :
			position(position), color(color), attenuation(attenuation), ambient(ambient)
	{ }

	const glm::vec3 &getPosition() const;

	void setPosition(const glm::vec3 &position);

	void move(const glm::vec3 &delta);

	const Color &getColor() const;

	void setColor(const Color &color);

	GLfloat getAmbient() const;

	void setAmbient(GLfloat ambientFactor);

	GLfloat getAttenuation() const;

	void setAttenuation(GLfloat attenuation);

	void insertInShader(const ShaderProgram &shaderProgram, GLuint num);
};

