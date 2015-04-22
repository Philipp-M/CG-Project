#include <GL/glew.h>
#include <iostream>
#include <map>
#include "ShaderProgram.hpp"


ShaderProgram::ShaderProgram(const std::string &name) : name(name)
{
    shaderProgramId = glCreateProgram();
}

void ShaderProgram::attachShader(const Shader &shader)
{
    glAttachShader(shaderProgramId, shader.getShaderId());
    shaders.push_back(shader);
}

void ShaderProgram::link()
{
    glLinkProgram(shaderProgramId);
    GLint infoLogLength = 0;

    glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        char *infoLog = new char[infoLogLength];
        GLint charsWritten = 0;

        glGetProgramInfoLog(shaderProgramId, infoLogLength, &charsWritten, infoLog);

        std::cerr << infoLog << std::endl;
        delete[] infoLog;
        glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &infoLogLength);
    }
    else
    {
        std::cerr << name << ": Program link failed exiting" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool ShaderProgram::vertexAttribPointer(const std::string &attribName, GLint size, GLenum type, GLsizei stride, const GLvoid *data,
                                        bool normalize) const
{
    glVertexAttribPointer(attributeLocation(attribName), size, type, normalize, stride, data);
}

GLuint ShaderProgram::attributeLocation(const std::string &name) const
{
    GLint loc = glGetAttribLocation(shaderProgramId, name.c_str());
    if (loc == -1)
    {
        std::cerr << "Attribute \"" << name << "\" not found in Program \"" << this->name << "\"\n";
        exit(EXIT_FAILURE);
    }
    return (GLuint) loc;
}

GLuint ShaderProgram::uniformLocation(const std::string &name) const
{
    GLint loc = glGetUniformLocation(shaderProgramId, name.c_str());
    if (loc == -1)
    {
        std::cerr << "Uniform \"" << name << "\" not found in Program \"" << this->name << "\"\n";
        exit(EXIT_FAILURE);
    }

    return (GLuint) loc;
}

ShaderProgram::ShaderProgram(const std::string &name, Shader vertex, Shader fragment) : name(name)
{
    attachShader(vertex);
    attachShader(fragment);
    link();
}
