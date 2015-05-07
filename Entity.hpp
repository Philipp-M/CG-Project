#pragma once

class Entity
{
public:
/**
     * moves the model based on the given vector, the movement is dependent of previous transformations
     */
	virtual void move(glm::vec3 delta) = 0;

	/**
	 * rotates the model based on the given vector, the movement is dependent of previous transformations
	 */
	virtual void rotate(glm::vec3 delta) = 0;

	/**
	 * scales the model based on the given vector, the movement is dependent of previous transformations
	 */
	virtual void scale(glm::vec3 delta) = 0;

	/**
	 * (uniformly) scales the model based on the given vector, the movement is dependent of previous transformations
	 */
	virtual void scale(GLfloat delta) = 0;

	/**
	 * sets the current transformation matrix to the identity matrix
	 */
	virtual void resetTransformationMatrix() = 0;

	/*
	 * returns the current transformation matrix
	 */
	virtual glm::mat4 getTransformationMatrix() const = 0;
};
