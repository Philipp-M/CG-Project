#pragma once

#include "TextureManager.hpp"
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct Material
{
	Material(const std::string &name, const glm::vec3 &color, const glm::vec3 &specColor, const Texture *cMap = NULL,
	         const Texture *nMap = NULL,
	         const Texture *sMap = NULL) :
			name(name), color(color), specColor(specColor), colorMap(cMap), normalMap(nMap), specularMap(sMap)
	{ }

	std::string name;
	glm::vec3 color; // is not really used since the model saves the color vertice wise
	glm::vec3 specColor;
	//glm::vec3 emission; // difficult to handle in simple opengl...
	const Texture *colorMap;
	const Texture *normalMap;
	const Texture *specularMap;
};

class MaterialManager
{
public:
	static MaterialManager &get();

	uint16_t addMaterial(Material* material);

	const Material *getByID(uint16_t id);

	const Material *getByName(const std::string &name);

	void deleteAllMaterials();

private:
	std::vector<Material*> materials;

	MaterialManager()
	{ }

	MaterialManager(const MaterialManager &other)
	{ } // non construction-copyable
	MaterialManager &operator=(const MaterialManager &)
	{ } // non copyable
};

inline void MaterialManager::deleteAllMaterials()
{
	materials.clear();
}

inline const Material *MaterialManager::getByName(const std::string &name)
{
	for (std::vector<Material*>::iterator it = materials.begin(); it != materials.end(); ++it)
		if ((*it)->name == name)
			return *it;
}

inline const Material *MaterialManager::getByID(uint16_t id)
{
	return materials.size() <= id ? NULL : materials[id];
}

inline uint16_t MaterialManager::addMaterial(Material* material)
{
	materials.push_back(material);
	return materials.size() - 1;
}

inline MaterialManager &MaterialManager::get()
{
	static MaterialManager instance;
	return instance;
}

