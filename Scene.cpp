#include <cstddef>
#include <cmath>
#include <iostream>
#include <fstream>
#include <glm/ext.hpp>
#include "Shader.hpp"
#include "Scene.hpp"
#include "ShaderProgramManager.hpp"
#include "tiny_obj_loader.hpp"
#include "picojson.hpp"
#include "TextureManager.hpp"
#include "MaterialManager.hpp"

class VertexBuilder
{
private:
	std::vector<GLuint> tempAdjVert;
	std::vector<glm::vec3> faceNormals;
	const std::vector<GLuint> &indices;
	std::vector<std::vector<GLuint> > adjVert;
	const std::vector<tinyobj::material_t> &materials;
	const tinyobj::mesh_t &mesh;

	glm::vec3 calculateFaceNormal(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3)
	{
		return glm::normalize(glm::cross(v3 - v2, v1 - v2));
	}

	glm::vec3 calculateTangent(const Model::Vertex3 &v0, const Model::Vertex3 &v1, const Model::Vertex3 &v2)
	{
		glm::vec2 deltaUV1 = v1.tex - v0.tex;
		glm::vec2 deltaUV2 = v2.tex - v0.tex;
		float f = 1.0f / (deltaUV1.s * deltaUV2.t - deltaUV2.s * deltaUV1.t);
		return (f * (deltaUV2.t * (v1.vertex - v0.vertex) - deltaUV1.t * (v2.vertex - v0.vertex)));
	}

	void calculateNormalsTangents(std::vector<Model::Vertex3> &vertices)
	{
		for (size_t f = 0; f < indices.size(); f += 3)
		{
			Model::Vertex3 &v0 = vertices[indices[f]];
			Model::Vertex3 &v1 = vertices[indices[f + 1]];
			Model::Vertex3 &v2 = vertices[indices[f + 2]];
			// calculate normals
			glm::vec3 fn = calculateFaceNormal(v0.vertex, v1.vertex, v2.vertex);
			v0.normal += fn;
			v1.normal += fn;
			v2.normal += fn;
			glm::vec3 t = calculateTangent(v0, v1, v2);
			v0.tangent += t;
			v1.tangent += t;
			v2.tangent += t;
		}
		for (size_t i; i < vertices.size(); i++)
		{
			vertices[i].normal = glm::normalize(vertices[i].normal);
			vertices[i].tangent = glm::normalize(vertices[i].tangent);
		}
	}

public:
	VertexBuilder(const tinyobj::mesh_t &mesh,
	              const std::vector<tinyobj::material_t> &materials) : indices(mesh.indices), mesh(mesh), materials(materials),
	                                                                   tempAdjVert(mesh.indices.size() / 3)
	{
	}

	void computeVertices(std::vector<Model::Vertex3> &vertices)
	{
		vertices.resize(mesh.positions.size());
		for (GLuint i = 0; i < mesh.positions.size() / 3; i++)
		{
			vertices[i] = Model::Vertex3(glm::vec3(mesh.positions[i * 3], mesh.positions[i * 3 + 1], mesh.positions[i * 3 + 2]),
			                             glm::vec3(),
			                             glm::vec3());
			if (mesh.texcoords.size() >= i * 2 + 1)
				vertices[i].tex = glm::vec2(mesh.texcoords[i * 2], 1.0 - mesh.texcoords[i * 2 + 1]);
		}
		calculateNormalsTangents(vertices);
	}
};


Scene::Scene(const std::string &filename)
{
	// create a new shader program based on the given file names "vertexshader.vs" and "fragmentshader.fs"
	// add the shader to the shaderProgramManager singleton, for maybe later independent use
	GLuint shaderId = ShaderProgramManager::get().addShaderProgram(
			new ShaderProgram("bumpShader", Shader("vertex", "bumpshader.vs", VERTEX), Shader("fragment", "bumpshader.fs", FRAGMENT)));
	ShaderProgramManager::get().addShaderProgram(
			new ShaderProgram("lightShader", Shader("vertex", "lightshader.vs", VERTEX), Shader("fragment", "lightshader.fs", FRAGMENT)));
	loadFromFile(filename);
}

Scene::~Scene()
{
	for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
		delete (*it);
	delete cameraSystem;
}

bool Scene::loadFromFile(const std::string &filename)
{
	bool foundObjFilename = false;
	bool foundMtlBasepath = false;
	bool foundCameraWidth = false;
	bool foundCameraHeight = false;
	bool foundCameraNearPlane = false;
	bool foundCameraFarPlane = false;
	bool foundCameraFOV = false;
	bool foundCameraPosition = false;
	bool foundCameraRotation = false;
	int cameraWidth;
	int cameraHeight;
	double cameraNearPlane;
	double cameraFarPlane;
	double cameraFOV;
	glm::vec3 cameraPosition;
	glm::vec3 cameraRotation;
	std::string objFilename;
	std::string mtlBasepath;
	std::ifstream file(filename.c_str());
	picojson::value v;
	file >> v;

	std::string err = picojson::get_last_error();
	if (!err.empty())
	{
		std::cerr << err << std::endl;
		exit(1);
	}

	// check if the type of the value is "object"
	if (!v.is<picojson::object>())
	{
		std::cerr << "JSON is not an object" << std::endl;
		exit(2);
	}
	const picojson::value::object &obj = v.get<picojson::object>();
	for (picojson::value::object::const_iterator i = obj.begin();
	     i != obj.end();
	     ++i)
	{
		if (i->first == "ObjFilename")
		{
			if (i->second.is<std::string>())
			{
				foundObjFilename = true;
				objFilename = i->second.get<std::string>();
			}
			else
			{
				std::cerr << "ObjFilename is not a string" << std::endl;
				exit(3);
			}
		}
		else if (i->first == "MtlBasepath")
		{
			if (i->second.is<std::string>())
			{
				foundMtlBasepath = true;
				mtlBasepath = i->second.get<std::string>();
			}
			else
			{
				std::cerr << "MtlBasepath is not a string" << std::endl;
				exit(4);
			}
		}
		else if (i->first == "Camera")
		{
			if (i->second.is<picojson::object>())
			{
				const picojson::value::object &camobj = i->second.get<picojson::object>();
				for (picojson::value::object::const_iterator j = camobj.begin(); j != camobj.end(); ++j)
				{
					if (j->first == "Width")
					{
						if (j->second.is<double>())
						{
							foundCameraWidth = true;
							cameraWidth = (int) j->second.get<double>();
						}
						else
						{
							std::cerr << "Camera.Width is not a number" << std::endl;
							exit(5);
						}
					}
					if (j->first == "Height")
					{
						if (j->second.is<double>())
						{
							foundCameraHeight = true;
							cameraHeight = (int) j->second.get<double>();
						}
						else
						{
							std::cerr << "Camera.Height is not a number" << std::endl;
							exit(6);
						}
					}
					if (j->first == "NearPlane")
					{
						if (j->second.is<double>())
						{
							foundCameraNearPlane = true;
							cameraNearPlane = j->second.get<double>();
						}
						else
						{
							std::cerr << "Camera.NearPlane is not a number" << std::endl;
							exit(6);
						}
					}
					if (j->first == "FarPlane")
					{
						if (j->second.is<double>())
						{
							foundCameraFarPlane = true;
							cameraFarPlane = j->second.get<double>();
						}
						else
						{
							std::cerr << "Camera.FarPlane is not a number" << std::endl;
							exit(6);
						}
					}
					if (j->first == "FOV")
					{
						if (j->second.is<double>())
						{
							foundCameraFOV = true;
							cameraFOV = j->second.get<double>();
						}
						else
						{
							std::cerr << "Camera.FOV is not a number" << std::endl;
							exit(7);
						}
					}
					if (j->first == "Position")
					{
						if (j->second.is<picojson::array>())
						{
							foundCameraPosition = true;
							const picojson::array &a = j->second.get<picojson::array>();
							int c = 0;
							for (picojson::array::const_iterator k = a.begin(); k != a.end(); ++k)
							{
								if (k->is<double>() && c < 3)
									cameraPosition[c] = k->get<double>();
								else if (!k->is<double>())
								{
									std::cerr << "Camera.Position is not an array of numbers" << std::endl;
									exit(8);
								}
								c++;
							}
							if (c != 3)
							{
								std::cerr << "Camera.Position is not an array of length 3" << std::endl;
								exit(9);
							}
						}
						else
						{
							std::cerr << "Camera.Position is not an array" << std::endl;
							exit(10);
						}
					}
					if (j->first == "Rotation")
					{
						if (j->second.is<picojson::array>())
						{
							foundCameraRotation = true;
							const picojson::array &a = j->second.get<picojson::array>();
							int c = 0;
							for (picojson::array::const_iterator k = a.begin(); k != a.end(); ++k)
							{
								if (k->is<double>() && c < 3)
									cameraRotation[c] = k->get<double>() * M_PI / 180.0;
								else if (!k->is<double>())
								{
									std::cerr << "Camera.Rotation is not an array of numbers" << std::endl;
									exit(8);
								}
								c++;
							}
							if (c != 3)
							{
								std::cerr << "Camera.Rotation is not an array of length 3" << std::endl;
								exit(9);
							}
						}
						else
						{
							std::cerr << "Camera.Rotation is not an array" << std::endl;
							exit(10);
						}
					}
				}
			}
			else
			{
				std::cerr << "Camera is not a json-Object" << std::endl;
				exit(5);
			}
		}
	}
	if (!(foundObjFilename && foundMtlBasepath && foundCameraWidth && foundCameraHeight && foundCameraNearPlane && foundCameraFarPlane &&
	      foundCameraFOV && foundCameraPosition && foundCameraRotation))
	{
		std::cerr << "some attributes are missing in the scene file" << std::endl;
		exit(3);
	}
	/************** finished parsing time for the creation of the scene **************/

	/************** load all the models **************/
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string::size_type slash = std::string::npos;
	std::string::size_type slashTmp = 0;
	while ((slashTmp = filename.find('/', slashTmp)) != std::string::npos)
		(slash = slashTmp) && (slashTmp++); // ugly but short ;)
	std::string basePath;
	std::cout << (filename.substr(0, slash + 1) + objFilename) << std::endl;
	if (slash != std::string::npos)
		err = tinyobj::LoadObj(shapes, materials, (filename.substr(0, slash + 1) + objFilename).c_str(),
		                       (filename.substr(0, slash + 1) + mtlBasepath).c_str());
	else
		err = tinyobj::LoadObj(shapes, materials, objFilename.c_str(), mtlBasepath.c_str());
	if (!err.empty())
	{
		std::cerr << err << std::endl;
		exit(EXIT_FAILURE);
	}
	deleteAllModels();
	TextureManager::get().deleteAllTextures();
	MaterialManager::get().deleteAllMaterials();
	TextureManager &tm = TextureManager::get();
	tm.loadTexture("./scene/textures/defaultDif.png");
	tm.loadTexture("./scene/textures/defaultSpec.png");
	tm.loadTexture("./scene/textures/defaultNorm.png");

	/************** load all the materials **************/

	for (std::vector<tinyobj::material_t>::iterator it = materials.begin(); it != materials.end(); ++it)
	{
		const Texture *texDif = NULL;
		const Texture *texSpec = NULL;
		const Texture *texNorm = NULL;

		if (slash != std::string::npos && it->diffuse_texname != "")
			texDif = tm.getByID(tm.loadTexture(filename.substr(0, slash + 1) + it->diffuse_texname));
		else if (it->diffuse_texname != "")
			texDif = tm.getByID(tm.loadTexture(it->diffuse_texname));

		if (slash != std::string::npos && it->specular_texname != "")
			texSpec = tm.getByID(tm.loadTexture(filename.substr(0, slash + 1) + it->specular_texname));
		else if (it->specular_texname != "")
			texSpec = tm.getByID(tm.loadTexture(it->specular_texname));

		if (slash != std::string::npos && it->normal_texname != "")
			texNorm = tm.getByID(tm.loadTexture(filename.substr(0, slash + 1) + it->normal_texname));
		else if (it->normal_texname != "")
			texNorm = tm.getByID(tm.loadTexture(it->normal_texname));
		const ShaderProgram *shader = it->name.find("light") != std::string::npos ?
		                              ShaderProgramManager::get().getShaderByName("lightShader") :
		                              ShaderProgramManager::get().getShaderByName("bumpShader");
		MaterialManager::get().addMaterial(new Material(it->name, Color::fromRGB(glm::vec3(it->diffuse[0], it->diffuse[1], it->diffuse[2])),
		                                                Color::fromRGB(glm::vec3(it->specular[0], it->specular[1], it->specular[2])), it->shininess + 0.01,
		                                                shader, texDif, texNorm,
		                                                texSpec));
	}
	/************** load all the vertices **************/
	for (size_t i = 0; i < shapes.size(); i++)
	{
		const std::string &name = shapes[i].name;
		const std::vector<GLuint> &iData = shapes[i].mesh.indices;
		std::vector<Model::Vertex3> vData(shapes[i].mesh.positions.size() / 3);
		VertexBuilder vb(shapes[i].mesh, materials);
		vb.computeVertices(vData);
		std::cout << "adding model: " << name << " with " << shapes[i].mesh.positions.size() << " vertices " << std::endl;
		// add the model and attach the first material(makes life easier then handling every connected material)
		// calculate centroid of mesh for the point light source
		glm::vec3 centroid = glm::vec3(0);
		for(int j = 0; j < vData.size(); j++)
			centroid += vData[0].vertex;
		centroid = centroid / (float)vData.size();
		if (shapes[i].mesh.material_ids.size() > 0)
		{
			Material* material = MaterialManager::get().getByNameNonConst(materials[shapes[i].mesh.material_ids[0]].name);
//			if(material->name.find("light") != std::string::npos)
//			{
//				addPointLight(PointLight(vData[0].vertex, Color::fromRGB(material->difColor), 1.0, 0.3));
//			}
			addModel(new Model(name, vData, iData, material, centroid));
		}
		else
			addModel(new Model(name, vData, iData, NULL, centroid));
	}
	/*********** setup Camera ***********/
	cameraSystem = new CameraSystem(cameraRotation, cameraPosition, cameraWidth, cameraHeight, cameraNearPlane, cameraFarPlane, cameraFOV);
}

void Scene::deleteAllModels()
{
	for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
		delete (*it);
	models = std::vector<Model *>();
}

void Scene::addModel(Model *model)
{
	if (model != NULL)
		models.push_back(model);
}

void Scene::draw()
{
//	shaderProgram->bind();
	//shaderProgram->setMatrixUniform4f("viewMatrix", cameraSystem->getViewMatrix());
	//shaderProgram->setMatrixUniform4f("projectionMatrix", cameraSystem->getProjectionMatrix());
	// init lights
//	shaderProgram->setUniform1i("numPointLights", pointLights.size());
//	shaderProgram->setUniform3f("cameraPosition", cameraSystem->getPosition());
//	for (int i = 0; i < pointLights.size(); i++)
//		pointLights[i].insertInShader(*shaderProgram, i);
	// draw all models
	// setup lights
	pointLights.clear();
	for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
	{
		if((*it)->isLight())
			pointLights.push_back(PointLight((*it)->getCentroid(),(*it)->getMaterial()->difColor));
	}
	for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
	{
		if ((*it) != NULL)
		{
			const ShaderProgram* shaderProgram = (*it)->getMaterial()->shader;
			shaderProgram->bind();
			if(shaderProgram->getName().find("light") == std::string::npos)
			{
				shaderProgram->setUniform1i("numPointLights", pointLights.size());
				shaderProgram->setUniform3f("cameraPosition", cameraSystem->getPosition());
				for (int i = 0; i < pointLights.size(); i++)
					pointLights[i].insertInShader(*shaderProgram, i);
			}
			//shaderProgram->setMatrixUniform4f("modelViewMatrix", cameraSystem->getViewMatrix() * (*it)->getTransformationMatrix());
			shaderProgram->setMatrixUniform4f("MVPMatrix", cameraSystem->getProjectionMatrix() * cameraSystem->getViewMatrix() *
			                                               (*it)->getTransformationMatrix());
			(*it)->draw(*shaderProgram);
			shaderProgram->unbind();
		}
	}
}

CameraSystem &Scene::getCameraSystem()
{
	return *cameraSystem;
}

Model *Scene::getModel(const std::string &name)
{
	for (std::vector<Model *>::iterator it = models.begin(); it != models.end(); ++it)
	{
		if (*it != NULL && (*it)->getName() == name)
			return *it;
	}
	return NULL;
}

const std::vector<Model *> &Scene::getModels()
{
	return models;
}

std::vector<PointLight> &Scene::getPointLights()
{
	return pointLights;
}

void Scene::addPointLight(const PointLight &light)
{
	if (pointLights.size() <= MAX_LIGHTS)
		pointLights.push_back(light);
}
