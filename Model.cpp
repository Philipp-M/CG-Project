#include <glm/ext.hpp>
#include <GL/glew.h>
#include "Model.hpp"
#include "ShaderProgram.hpp"

Model::Model(const std::string &name, const std::vector<Vertex3> &verticeData, const std::vector<GLuint> &indices, Material *mat,
             glm::vec3 centroid) :
		name(name), verticeData(verticeData), indices(indices), material(mat), transMat(glm::mat4(1.0)), centroid(centroid)
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

void Model::draw(const ShaderProgram &shaderProgram, const glm::vec3 &cameraPosition)
{
	glBindBuffer(GL_ARRAY_BUFFER, idVert);
	glEnableVertexAttribArray(shaderProgram.attributeLocation("position"));
	shaderProgram.vertexAttribPointer("position", 3, GL_FLOAT, sizeof(Vertex3), 0, false);
	if (!isLight())
	{
		glEnableVertexAttribArray(shaderProgram.attributeLocation("normal"));
		shaderProgram.vertexAttribPointer("normal", 3, GL_FLOAT, sizeof(Vertex3), (void *) (sizeof(glm::vec3)), false);
		glEnableVertexAttribArray(shaderProgram.attributeLocation("tangent"));
		shaderProgram.vertexAttribPointer("tangent", 3, GL_FLOAT, sizeof(Vertex3), (void *) (2 * sizeof(glm::vec3)), false);
		glEnableVertexAttribArray(shaderProgram.attributeLocation("texCoord"));
		shaderProgram.vertexAttribPointer("texCoord", 2, GL_FLOAT, sizeof(Vertex3), (void *) (3 * sizeof(glm::vec3)), false);
		shaderProgram.setMatrixUniform4f("modelMatrix", getTransformationMatrix(cameraPosition));
		shaderProgram.setMatrixUniform3f("normalMatrix", glm::transpose(glm::inverse(glm::mat3(
				getTransformationMatrix(cameraPosition)))));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIndices);
	if (material != NULL)
	{
		shaderProgram.setUniform3f("difColor", material->difColor.getRGB());
		if (!isLight())
		{
			shaderProgram.setUniform3f("specColor", material->specColor.getRGB());
			shaderProgram.setUniform1f("shininess", material->shininess);
			// bind the texture(s)
			// diffuse texture
			glActiveTexture(GL_TEXTURE0);
			if (material->colorMap != NULL)
				glBindTexture(GL_TEXTURE_2D, material->colorMap->id);
			else
				glBindTexture(GL_TEXTURE_2D, TextureManager::get().getByID(0)->id);
			shaderProgram.setUniform1i("diffuseTex", 0);
			// specular texture
			if (material->specularMap != NULL)
			{
				shaderProgram.setUniform1i("useSpecularMapping", 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->specularMap->id);
				shaderProgram.setUniform1i("specularTex", 1);
			}
			else
				shaderProgram.setUniform1i("useSpecularMapping", 0);
			// normal texture
			if (material->normalMap != NULL)
			{
				shaderProgram.setUniform1i("useNormalMapping", 1);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, material->normalMap->id);
				shaderProgram.setUniform1i("normalTex", 2);
			}
			else
				shaderProgram.setUniform1i("useNormalMapping", 0);

		}
	}

	GLint size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, (GLsizei) (size / sizeof(GLuint)), GL_UNSIGNED_INT, 0);
}

glm::mat4 Model::getTransformationMatrix(const glm::vec3 &cameraPosition) const
{
	if (isBillboardCylinder())
		return billboardCylinder(cameraPosition);
	else if (isBillboardSphere())
		return billboardSphere(cameraPosition);
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

glm::mat4 Model::billboardCylinder(const glm::vec3 &cameraPosition) const
{

	glm::vec3 lookAt = glm::vec3(0, 1, 0);
	glm::vec3 position = glm::vec3(transMat*glm::vec4(getCentroid(),1));
	glm::vec3 objToCamProj = -cameraPosition - position;
	objToCamProj.z = 0;
	objToCamProj = glm::normalize(objToCamProj);
	glm::vec3 upAux = glm::cross(lookAt, objToCamProj);
	GLfloat angleCos = glm::dot(lookAt, objToCamProj);
	glm::mat4 bMat = glm::translate(getCentroid());
	bMat *= glm::rotate((float) acos(angleCos), upAux);
	return bMat;
}

glm::mat4 Model::billboardSphere(const glm::vec3 &cameraPosition) const
{
	// cylindrical part
	glm::vec3 lookAt = glm::vec3(0, 1, 0);
	glm::vec3 position = glm::vec3(transMat*glm::vec4(getCentroid(),1));
	glm::vec3 objToCamProj = -cameraPosition - position;
	objToCamProj.z = 0;
	objToCamProj = glm::normalize(objToCamProj);
	glm::vec3 upAux = glm::cross(lookAt, objToCamProj);
	GLfloat angleCos = glm::dot(lookAt, objToCamProj);
	glm::mat4 bMat = glm::translate(position);
	bMat *= glm::rotate((float) acos(angleCos), upAux);

	// spherical part

	glm::vec3 objToCam = glm::normalize(-cameraPosition - position);
	angleCos = glm::dot(objToCamProj, objToCam);
	if (objToCam.z < 0)
		bMat *= glm::rotate((float) acos(angleCos), glm::vec3(-1, 0, 0));
	else
		bMat *= glm::rotate((float) acos(angleCos), glm::vec3(1, 0, 0));
	return bMat;
}

void Model::rotateAroundAxis(glm::vec3 a, float w)
{
	transMat *= glm::rotate(w, a);
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

Material *Model::getMaterial()
{
	return material;
}

bool Model::isBillboard() const
{
	return material != NULL ? material->name.find("billboard") != std::string::npos : false;
}

bool Model::isBillboardCylinder() const
{
	return material != NULL ? material->name.find("billboardC") != std::string::npos : false;
}

bool Model::isBillboardSphere() const
{
	return material != NULL ? material->name.find("billboardS") != std::string::npos : false;
}

bool Model::isLight() const
{
	return material != NULL ? material->name.find("light") != std::string::npos : false;
}

const glm::vec3 &Model::getCentroid() const
{
	return centroid;
}

