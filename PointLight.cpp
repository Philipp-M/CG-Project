#include <sstream>
#include <iostream>
#include "PointLight.hpp"

#define SSTR(x) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

const glm::vec3 &PointLight::getPosition() const
{
	return position;
}

void PointLight::setPosition(const glm::vec3 &position)
{
	PointLight::position = position;
}

void PointLight::move(const glm::vec3 &delta)
{
	position += delta;
}

const Color &PointLight::getColor() const
{
	return color;
}

void PointLight::setColor(const Color &color)
{
	PointLight::color = color;
}

GLfloat PointLight::getIntensity() const
{
	return intensity;
}

void PointLight::setIntensity(GLfloat intensity)
{
	PointLight::intensity = intensity;
}

GLfloat PointLight::getAmbient() const
{
	return ambient;
}

void PointLight::setAmbient(GLfloat ambient)
{
	PointLight::ambient = ambient;
}

GLfloat PointLight::getAttenuation() const
{
	return attenuation;
}

void PointLight::setAttenuation(GLfloat attenuation)
{
	PointLight::attenuation = attenuation;
}

void PointLight::insertInShader(const ShaderProgram &shaderProgram, GLuint num)
{
	shaderProgram.setUniform3f("allPointLights[" + SSTR(num) + "].position", position);
	shaderProgram.setUniform3f("allPointLights[" + SSTR(num) + "].colorIntensity", intensity * color.getRGB());
	shaderProgram.setUniform1f("allPointLights[" + SSTR(num) + "].attenuation", attenuation);
	shaderProgram.setUniform1f("allPointLights[" + SSTR(num) + "].ambient", ambient);
}

