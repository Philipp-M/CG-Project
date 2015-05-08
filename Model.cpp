#include <glm/ext.hpp>
#include <GL/glew.h>
#include "Model.hpp"
#include "ShaderProgram.hpp"

Model::Model(const std::string &name, const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices, const Material* mat) :
		name(name), verticeData(verticeData), indices(indices), material(mat), transMat(glm::mat4(1.0))
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
		shaderProgram->setMatrixUniform4f("ModelMatrix", getTransformationMatrix());
		glEnableVertexAttribArray(shaderProgram->attributeLocation("position"));
		glBindBuffer(GL_ARRAY_BUFFER, idVert);
		shaderProgram->vertexAttribPointer("position", 3, GL_FLOAT, sizeof(Vertex3), 0, false);
//		glEnableVertexAttribArray(shaderProgram->attributeLocation("normal"));
//		shaderProgram->vertexAttribPointer("normal", 3, GL_FLOAT, sizeof(Vertex3), (void *) (sizeof(glm::vec3)), false);
		glEnableVertexAttribArray(shaderProgram->attributeLocation("color"));
		shaderProgram->vertexAttribPointer("color", 3, GL_FLOAT, sizeof(Vertex3), (void *) (2 * sizeof(glm::vec3)), false);
		glEnableVertexAttribArray(shaderProgram->attributeLocation("texCoord"));
		shaderProgram->vertexAttribPointer("texCoord", 2, GL_FLOAT, sizeof(Vertex3), (void *) (3 * sizeof(glm::vec3)), false);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
		// bind the texture(s)
		// diffuse texture
		glActiveTexture(GL_TEXTURE0);
		if(material != NULL && material->colorMap != NULL)
			glBindTexture(GL_TEXTURE_2D, material->colorMap->id);
		else
			glBindTexture(GL_TEXTURE_2D, TextureManager::get().getByID(0)->id);
		shaderProgram->setUniform1i("diffuse", 0);
		// specular texture
		glActiveTexture(GL_TEXTURE1);
		if(material != NULL && material->specularMap != NULL)
			glBindTexture(GL_TEXTURE_2D, material->specularMap->id);
		else
			glBindTexture(GL_TEXTURE_2D, TextureManager::get().getByID(1)->id);
		// normal texture
		glActiveTexture(GL_TEXTURE2);
		if(material != NULL && material->normalMap != NULL)
			glBindTexture(GL_TEXTURE_2D, material->normalMap->id);
		else
			glBindTexture(GL_TEXTURE_2D, TextureManager::get().getByID(2)->id);
		GLint size;
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, (GLsizei) (size / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
	}
}

glm::mat4 Model::getTransformationMatrix() const
{
	return transMat;
}

void Model::scale(glm::vec3 delta)
{
	transMat *= glm::scale(delta);
}

void Model::scale(GLfloat factor)
{
	scale(glm::vec3(factor, factor, factor));
}

void Model::rotate(glm::vec3 delta)
{
	transMat *= glm::rotate(delta.x, glm::vec3(1, 0, 0)) * glm::rotate(delta.y, glm::vec3(0, 1, 0)) *
	            glm::rotate(delta.z, glm::vec3(0, 0, 1));
}

void Model::move(glm::vec3 delta)
{
	transMat *= glm::translate(delta);
}

const std::string &Model::getName()
{
	return name;
}

void Model::resetTransformationMatrix()
{
	transMat = glm::mat4(1.0);
}

