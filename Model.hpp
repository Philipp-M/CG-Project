#pragma once

#include <glm/glm.hpp>
#include <GL/gl.h>
#include <vector>
#include "Entity.hpp"
#include "ShaderProgram.hpp"

class Model : public Entity
{
public:
    struct Vertex3
    {
        Vertex3(glm::vec3 v, glm::vec3 c) : vertex(v), color(c)
        { }

        glm::vec3 vertex;
        glm::vec3 color;
    };

private:
    GLuint idVert;
    GLuint idIndices;
    std::string name;
    /**
     * maybe not needed and done in Scene...
     */
    GLuint shaderProgramId;
    bool bufferObjectsloaded;
    std::vector<Vertex3> verticeData;
    std::vector<GLuint> indices;
    glm::mat4 transMat;
public:
    Model(const std::string &name, const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices);

    void refreshBuffers();

    void draw(const ShaderProgram *shaderProgram);

    const std::string &getName();

    /*** implemented from the interface Entity ***/
    void move(glm::vec3 delta);

    void rotate(glm::vec3 delta);

    void scale(glm::vec3 delta);

    void scale(GLfloat delta);

    void resetTransformationMatrix();

    const glm::mat4 &getTransformationMatrix();
};

