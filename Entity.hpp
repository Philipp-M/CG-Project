#pragma once
class Entity
{
public:
    virtual void move(glm::vec3 delta) = 0;
    virtual void rotate(glm::vec3 delta) = 0;
    virtual void scale(glm::vec3 factor) = 0;
    virtual void scale(GLfloat factor) = 0;
    virtual const glm::mat4& getTransformationMatrix() = 0;
    virtual void resetTransformationMatrix() = 0;
};
