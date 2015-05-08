#pragma once

#include <glm/glm.hpp>
#include <GL/gl.h>
#include <vector>
#include "Entity.hpp"
#include "ShaderProgram.hpp"
#include "MaterialManager.hpp"

class Model : public Entity
{
public:
	/**
	 * vertex structure contains the vertex and the corresponding color
	 */
	struct Vertex3
	{
		Vertex3(glm::vec3 v = glm::vec3(), glm::vec3 n = glm::vec3(), glm::vec3 c = glm::vec3(), glm::vec2 t = glm::vec2()) : vertex(v),
		                                                                                                                      normal(n),
		                                                                                                                      color(c),
		                                                                                                                      tex(t)
		{ }

		glm::vec3 vertex;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 tex;
	};

private:
	GLuint idVert;
	GLuint idIndices;
	std::string name;
	bool bufferObjectsloaded;
	std::vector<Vertex3> verticeData;
	std::vector<GLuint> indices;
	const Material *material;
	glm::mat4 transMat;
public:
	/**
	 * constructs a model with the given name, verticeData and indices
	 */
	Model(const std::string &name, const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices, const Material *mat);

	/**
	 * refreshes the buffer objects based on the local data(verticeData and indices)
	 */
	void refreshBuffers();

	/**
	 * draws the model to the screen based on the shaderProgram,
	 * the shaderProgram has to have the uniform variables:
	 * mat4 ModelMatrix, atrVec3 Position, atrVec3 Color
	 */
	void draw(const ShaderProgram *shaderProgram);

	/**
	 * returns the name of the model
	 */
	const std::string &getName();

	/*** implemented from the interface Entity ***/

	void move(glm::vec3 delta);

	void rotate(glm::vec3 delta);

	void scale(glm::vec3 delta);

	void scale(GLfloat delta);

	void resetTransformationMatrix();

	glm::mat4 getTransformationMatrix() const;
};

