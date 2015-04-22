#pragma once
#include <GL/gl.h>
#include <string>

class Shader
{
public:
    enum ShaderType
    {
        VERTEX, FRAGMENT
    };
private:
    GLuint shaderId;

    std::string name;


    ShaderType type;

    GLuint load(const std::string &filename);

public:
    Shader(const std::string &name, const std::string &filename, ShaderType type);

    ShaderType const getType() const;

    GLuint getShaderId() const;

    const std::string &getName() const;

    void setName(std::string &name);

private:
    void compile(const std::string &source);
};

