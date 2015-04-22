#pragma once

#include <string>
#include <vector>
#include <GL/gl.h>
#include "Shader.hpp"

class ShaderProgram
{
private:
public:
private:
    GLuint shaderProgramId;
    std::string name;
    std::vector<Shader> shaders;
public:
    ShaderProgram(const std::string &name);

    ShaderProgram(const std::string &name, Shader vertex, Shader fragment);

    void attachShader(const Shader &shader);

    void link();

    bool vertexAttribPointer(const std::string &attribName, GLint size, GLenum type, GLsizei stride, const GLvoid *data,
                             bool normalize) const;

    GLuint attributeLocation(const std::string &str) const;

    GLuint uniformLocation(const std::string &str) const;

    GLuint getId() const;

    const std::string &getName() const;
};

inline GLuint ShaderProgram::getId() const
{
    return shaderProgramId;
}

inline const std::string &ShaderProgram::getName() const
{
    return name;
}

