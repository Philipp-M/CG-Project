#pragma once

#include "SOIL/SOIL.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <GL/gl.h>
#include <iostream>

/**
 * simple texture structure, containing pixeldata and the size of the image
 */
struct Texture
{
	std::string name;
	size_t width;
	size_t height;
	uint8_t *pixels;
	GLuint id;
};

/**
 * very simple texture manager singleton, its quite easy to understand...
 */
class TextureManager
{
public:
	/**
	 * if the texture manager gets destroyed, it also deletes all the textures it holds...
	 */
	virtual ~TextureManager()
	{
		for (std::vector<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
			free((*it).pixels);
	}

	static TextureManager &get();

	/**
	 * loads a texture
	 * returns the id of the texture...
	 */
	int loadTexture(const std::string &filename);

	const Texture *getByID(uint16_t id);

	int getIDByName(const std::string &name);

	const Texture *getByName(const std::string &name);

	void deleteAllTextures();

private:
	std::vector<Texture> textures;

	// private constructor to prevent instancing
	TextureManager()
	{ }

	TextureManager(const TextureManager &other)
	{ } // non construction-copyable
	TextureManager &operator=(const TextureManager &)
	{ } // non copyable
};

inline TextureManager &TextureManager::get()
{
	static TextureManager instance;
	return instance;
}

inline const Texture *TextureManager::getByID(uint16_t id)
{
	return textures.size() <= id ? NULL : &textures[id];
}

inline int TextureManager::getIDByName(const std::string &name)
{
	for (int i = 0; i < textures.size(); i++)
		if (textures[i].name.compare(name) == 0)
			return i;
	return -1;
}

inline const Texture *TextureManager::getByName(const std::string &name)
{
	int t = getIDByName(name);
	return t == -1 ? NULL : getByID(t);
}

inline void TextureManager::deleteAllTextures()
{
	for (std::vector<Texture>::iterator it = textures.begin(); it != textures.end(); ++it)
		free((*it).pixels);
	textures.clear();
}


inline int TextureManager::loadTexture(const std::string &filename)
{
	// search for already exisiting texture
	int t = getIDByName(filename);
	if (t != -1)
		return t;
	Texture tex;
	int width;
	int height;
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);

	tex.name = filename;
	tex.pixels = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	tex.width = width;
	tex.height = height;

	if (tex.pixels == NULL)
	{
		std::cout << "An error occurred while loading image '" << filename << "'." << std::endl;
		exit(EXIT_FAILURE);
	}
	// glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // necessary?

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width, tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.pixels);
	textures.push_back(tex);
	return textures.size() - 1;
}
