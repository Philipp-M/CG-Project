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
	virtual ~TextureManager();

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
	std::vector<Texture*> textures;

	// private constructor to prevent instancing
	TextureManager()
	{ }

	TextureManager(const TextureManager &other)
	{ } // non construction-copyable
	TextureManager &operator=(const TextureManager &)
	{ } // non copyable
};

inline TextureManager::~TextureManager()
{
	deleteAllTextures();
}

inline TextureManager &TextureManager::get()
{
	static TextureManager instance;
	return instance;
}

inline const Texture *TextureManager::getByID(uint16_t id)
{
	return textures.size() <= id ? NULL : textures[id];
}

inline int TextureManager::getIDByName(const std::string &name)
{
	for (int i = 0; i < textures.size(); i++)
		if (textures[i]->name == name)
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
	for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		free((*it)->pixels);
		delete *it;
	}
	textures.clear();
}


inline int TextureManager::loadTexture(const std::string &filename)
{
	// search for already exisiting texture
	int t = getIDByName(filename);
	if (t != -1)
		return t;
	Texture *tex = new Texture();
	int width;
	int height;
	glGenTextures(1, &tex->id);
	glBindTexture(GL_TEXTURE_2D, tex->id);

	tex->name = filename;
	tex->pixels = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	tex->width = width;
	tex->height = height;

	if (tex->pixels == NULL)
	{
		std::cout << "An error occurred while loading image '" << filename << "'." << std::endl;
		exit(EXIT_FAILURE);
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // necessary?

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	textures.push_back(tex);
	return textures.size() - 1;
}
