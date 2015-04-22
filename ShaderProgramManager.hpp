#pragma once

#include <GL/gl.h>
#include <string>
#include <map>
#include "ShaderProgram.hpp"

class ShaderProgramManager
{
private:
    std::map<GLuint, ShaderProgram *> shaderProgramMap;
    std::map<std::string, GLuint> namesIdMap;

    ShaderProgramManager()
    { }
    ~ShaderProgramManager();

public:
    static ShaderProgramManager &get();

    const ShaderProgram *getShaderById(GLuint id);

    const ShaderProgram *getShaderByName(const std::string &name);

    GLuint addShaderProgram(ShaderProgram *shaderProgram);
};

ShaderProgramManager &ShaderProgramManager::get()
{
    static ShaderProgramManager instance;
    return instance;
}

const ShaderProgram *ShaderProgramManager::getShaderById(GLuint id)
{
    std::map<GLuint, ShaderProgram *>::const_iterator it;
    it = shaderProgramMap.find(id);
    if (it != shaderProgramMap.end())
    {
        return shaderProgramMap[id];
    }
    return nullptr;
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
}

inline ShaderProgramManager::~ShaderProgramManager()
{
    for(auto kv : shaderProgramMap)
        delete kv.second;
}
