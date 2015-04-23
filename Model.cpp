#include <glm/ext.hpp>
#include <GL/glew.h>
#include "Model.hpp"
#include "ShaderProgram.hpp"

Model::Model(const std::string& name,const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices) :
        name(name),verticeData(verticeData), indices(indices)
{
    refreshBuffers();
}

void Model::refreshBuffers()
{
    if (bufferObjectsloaded)
    {
        glDeleteBuffers(1, &idVert);
        glDeleteBuffers(1, &idIndices);
    }
    glGenBuffers(1, &idVert);
    glGenBuffers(1, &idIndices);
    glBindBuffer(GL_ARRAY_BUFFER, idVert);
    glBufferData(GL_ARRAY_BUFFER, verticeData.size() * sizeof(Vertex3), &verticeData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);
    bufferObjectsloaded = true;
}

void Model::draw(const ShaderProgram *shaderProgram)
{
    if (shaderProgram != NULL)
    {
        /**
         * optimization possible with caching the matrix
         */
        shaderProgram->setMatrixUniform4f("ModelMatrix", getTransformationMatrix());
        glEnableVertexAttribArray(shaderProgram->attributeLocation("Position"));
        glBindBuffer(GL_ARRAY_BUFFER, idVert);
        shaderProgram->vertexAttribPointer("Position", 3, GL_FLOAT, sizeof(Vertex3), 0,false);
        glEnableVertexAttribArray(shaderProgram->attributeLocation("Color"));
        shaderProgram->vertexAttribPointer("Color", 3, GL_FLOAT, sizeof(Vertex3), (void*)sizeof(glm::vec3),false);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);

        GLint size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, (GLsizei) (size/sizeof(GLuint)), GL_UNSIGNED_INT, 0);
    }
}
glm::mat4 Model::getTransformationMatrix()
{
    glm::mat4 retMat = glm::translate(glm::vec3(position.x, position.y, position.z));
    retMat *= glm::rotate(rotX, glm::vec3(1, 0, 0)) * glm::rotate(rotY, glm::vec3(0, 1, 0)) *
              glm::rotate(rotZ, glm::vec3(0, 0, 1));
    //retMat *= glm::translate(glm::vec3(position.x, position.y, position.z));
    return retMat;

}
void Model::rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
    this->rotX += rotX;
    this->rotY += rotY;
    this->rotZ += rotZ;
}

void Model::setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ)
{
    this->rotX = rotX;
    this->rotY = rotY;
    this->rotZ = rotZ;
}

glm::vec3 Model::getRotation() const
{
    return glm::vec3(rotX, rotY, rotZ);
}

glm::vec3 Model::getPosition() const
{
    return position;
}

void Model::move(glm::vec3 delta)
{
    position += delta;
}

void Model::setPosition(glm::vec3 position)
{
    this->position = position;
}


const std::string &Model::getName()
{
    return name;
}
