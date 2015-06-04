#pragma once

#include <glm/glm.hpp>

class Color
{
private:
	glm::vec3 rgb;

	Color()
	{ }

public:
	static Color fromHSV(glm::vec3 hsv);

	/**
	 * h is hue s is saturation and v is value
	 * h is between 0 and 360
	 * s is between 0 and 1
	 * and v can be a value greater than 0
	 */
	static Color fromHSV(float h, float s, float v);

	/**
	 * just a rgb constructor where r is red, g is green and b is blue
	 */
	static Color fromRGB(glm::vec3 rgb);

	/**
	 * just a rgb constructor where r is red, g is green and b is blue
	 */
	static Color fromRGB(float r, float g, float b);

	/**
	 * returns a 3 dimensional vector with the hsv values of the color
	 */
	glm::vec3 getHSV();

	/**
	 * returns a 3 dimensional vector with the rgb values of the color
	 */
	glm::vec3 getRGB();

	void setHue(float h);

	void setSaturation(float s);

	void setValue(float v);

};

