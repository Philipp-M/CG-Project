#pragma once

#include <cstddef>
#include <GL/gl.h>
#include <string>
#include <map>
#include <stddef.h>
#include "ShaderProgram.hpp"

class ShaderProgramManager
{
private:
	std::map<GLuint, ShaderProgram *> shaderProgramMap;
	std::map<std::string, GLuint> namesIdMap;

	ShaderProgramManager()
	{ }

public:
	~ShaderProgramManager();

	static ShaderProgramManager &get();

	const ShaderProgram *getShaderById(GLuint id);

	const ShaderProgram *getShaderByName(const std::string &name);

	GLuint addShaderProgram(ShaderProgram *shaderProgram);
};

inline ShaderProgramManager &ShaderProgramManager::get()
{
	static ShaderProgramManager instance;
	return instance;
}

inline const ShaderProgram *ShaderProgramManager::getShaderById(GLuint id)
{
	std::map<GLuint, ShaderProgram *>::const_iterator it;
	it = shaderProgramMap.find(id);
	if (it != shaderProgramMap.end())
	{
		return shaderProgramMap[id];
	}
	return NULL;
}

inline const ShaderProgram *ShaderProgramManager::getShaderByName(const std::string &name)
{
	std::map<std::string, GLuint>::const_iterator it;
	it = namesIdMap.find(name);
	if (it != namesIdMap.end())
	{
		return getShaderById(namesIdMap[name]);
	}
}

inline GLuint ShaderProgramManager::addShaderProgram(ShaderProgram *shaderProgram)
{
	if (shaderProgram->getId() > 0 && !shaderProgram->getName().empty())
	{
		namesIdMap[shaderProgram->getName()] = shaderProgram->getId();
		shaderProgramMap[shaderProgram->getId()] = shaderProgram;
	}
	return shaderProgram->getId();
}

inline ShaderProgramManager::~ShaderProgramManager()
{
	for (std::map<GLuint, ShaderProgram *>::iterator it = shaderProgramMap.begin(); it != shaderProgramMap.end(); ++it)
		delete it->second;
}
