#pragma once
class Entity
{
public:
    virtual void setPosition(glm::vec3 position) = 0;
    virtual void move(glm::vec3 delta) = 0;
    virtual glm::vec3 getPosition() const = 0;
    virtual glm::vec3 getRotation() const = 0;
    virtual void setRotation(GLfloat rotX, GLfloat rotY, GLfloat rotZ) = 0;
    virtual void rotate(GLfloat rotX, GLfloat rotY, GLfloat rotZ) = 0;
};
