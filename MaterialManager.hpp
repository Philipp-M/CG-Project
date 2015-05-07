#pragma once

#include "TextureManager.hpp"
#include <vector>
#include <string>

#include <glm/glm.hpp>

struct Material
{
	Material(const std::string &name, const glm::vec3 &color, const glm::vec3 &emission, const Texture *cMap, const Texture *nMap,
	         const Texture *sMap) :
			name(name), color(color), emission(emission), colorMap(cMap), normalMap(nMap), specularMap(sMap)
	{ }

	std::string name;
	glm::vec3 color;
	glm::vec3 emission;
	const Texture *colorMap;
	const Texture *normalMap;
	const Texture *specularMap;

};

class MaterialManager
{
public:
	static MaterialManager &get()
	{
		static MaterialManager instance;
		return instance;
	}

	uint16_t addMaterial(const Material &material)
	{
		materials.push_back(material);
		return materials.size() - 1;
	}

	const Material *getByID(uint16_t id)
	{ return materials.size() <= id ? NULL : &materials[id]; }

private:
	std::vector<Material> materials;

	MaterialManager()
	{ }
};

