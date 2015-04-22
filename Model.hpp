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
        Vertex3(glm::vec3 v, glm::vec3 c) :vertex(v), color(c){}
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
    glm::vec3 position;
    GLfloat rotX;
    GLfloat rotY;
    GLfloat rotZ;
public:
    Model(const std::string& name, const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices);

    void refreshBuffers();

    void draw(const ShaderProgram *shaderProgram);

    /*** implemented from the interface Entity ***/
    void setPosition(glm::vec3 position);

    void move(glm::vec3 delta);

    glm::vec3 getPosition() const;

    glm::vec3 getRotation() const;

    void setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ);

    void rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ);

    glm::mat4 getTransformationMatrix();

    const std::string& getName();

};

