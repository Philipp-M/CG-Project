#pragma once

#include "TextureManager.hpp"
#include <vector>
#include <string>
#include <Eigen/Dense>

struct Material
{
	Material(const std::string& name, const Eigen::Vector3f& color, const Eigen::Vector3f& emission, const Texture* cMap, const Texture* nMap, const Texture* sMap) :
			name(name), color(color), emission(emission), colorMap(cMap), normalMap(nMap), specularMap(sMap) { }
	std::string name;
	Eigen::Vector3f color;

	Eigen::Vector3f emission;
	const Texture* colorMap;
	const Texture* normalMap;
	const Texture* specularMap;

};

class MaterialManager
{
public:
	static MaterialManager& get()
	{
		static MaterialManager instance;
		return instance;
	}
	uint16_t addMaterial(const Material& material) { materials.push_back(material); return materials.size()-1; }
	const Material* getByID (uint16_t id) { return materials.size() <= id ? NULL : &materials[id]; }
private:
	std::vector<Material> materials;
	MaterialManager() { }
};

