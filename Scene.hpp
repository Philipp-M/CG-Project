#pragma once

#include <string>
#include <vector>
#include "Model.hpp"
#include "CameraSystem.hpp"
#include "PointLight.hpp"

const GLuint MAX_LIGHTS = 10;

class Scene
{
private:
	CameraSystem *cameraSystem;
	std::vector<Model *> models;
	std::vector<PointLight> pointLights;
	/**
	 * temporary variable will hopefully in future be loaded within the scene loader or
	 * with functions that recognize which material is inside the obj file
	 */
	const ShaderProgram *shaderProgram;

	void deleteAllModels();

public:
	Scene(const std::string &filename);

	~Scene();

	/**
	 * draws all models from the viewing point seen(camera)
	 */
	void draw();

	/**
	 * adds a model to the scene
	 */
	void addModel(Model *model);

	/**
	 * adds a model to the scene
	 */
	void addPointLight(const PointLight &light);

	/**
	 * returns a reference to the array where all models are held in
	 */
	const std::vector<Model *> &getModels();

	std::vector<PointLight> &getPointLights();

	/**
	 * loads a scene based on a scene file written in JSON *WIP*
	 */

	bool loadFromFile(const std::string &filename);

	/**
	 * returns a reference to the camera for external control
	 */
	CameraSystem &getCameraSystem();

	/**
	 * returns a reference to the camera for external control
	 */
	Model *getModel(const std::string &name);

};
