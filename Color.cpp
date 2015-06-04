#include "Color.hpp"
#include <algorithm>

Color Color::fromHSV(glm::vec3 hsv)
{
	float hh, p, q, t, ff;
	int i;
	Color c;
	c.rgb = glm::vec3(0);

	if (hsv.y <= 0.0)
	{
		c.rgb.r = hsv.z;
		c.rgb.g = hsv.z;
		c.rgb.b = hsv.z;
		return c;
	}
	hh = hsv.x;
	if (hh >= 360.0f)
		hh = 0.0f;
	hh /= 60.0f;
	i = (int) hh;
	ff = hh - i;
	p = hsv.z * (1.0f - hsv.y);
	q = hsv.z * (1.0f - (hsv.y * ff));
	t = hsv.z * (1.0f - (hsv.y * (1.0f - ff)));

	switch (i)
	{
		case 0:
			c.rgb.r = hsv.z;
	        c.rgb.g = t;
	        c.rgb.b = p;
	        break;
		case 1:
			c.rgb.r = q;
	        c.rgb.g = hsv.z;
	        c.rgb.b = p;
	        break;
		case 2:
			c.rgb.r = p;
	        c.rgb.g = hsv.z;
	        c.rgb.b = t;
	        break;
		case 3:
			c.rgb.r = p;
	        c.rgb.g = q;
	        c.rgb.b = hsv.z;
	        break;
		case 4:
			c.rgb.r = t;
	        c.rgb.g = p;
	        c.rgb.b = hsv.z;
	        break;
		case 5:
		default:
			c.rgb.r = hsv.z;
	        c.rgb.g = p;
	        c.rgb.b = q;
	        break;
	}
	return c;
}

Color Color::fromHSV(float h, float s, float v)
{
	return fromHSV(glm::vec3(h, s, v));
}

Color Color::fromRGB(glm::vec3 rgb)
{
	Color c;
	c.rgb = rgb;
	return c;

}

Color Color::fromRGB(float r, float g, float b)
{
	return fromRGB(glm::vec3(r, g, b));
}

glm::vec3 Color::getHSV()
{
	float m = std::max(std::max(rgb.r, rgb.g), rgb.b);
	float n = std::min(std::min(rgb.r, rgb.g), rgb.b);
	glm::vec3 hsv;
	if (m == n)
		hsv.x = 0;
	else if (m == rgb.r)
		hsv.x = 60 * (0 + (rgb.g - rgb.b) / (m - n));
	else if (m == rgb.g)
		hsv.x = 60 * (2 + (rgb.b - rgb.r) / (m - n));
	else if (m == rgb.b)
		hsv.x = 60 * (4 + (rgb.r - rgb.g) / (m - n));
	if (hsv.x < 0)
		hsv.x += 360;
	if (m == 0)
		hsv.y = 0;
	else
		hsv.y = (m - n) / m;
	hsv.z = m;
	return hsv;
}

glm::vec3 Color::getRGB()
{
	return rgb;
}

void Color::setHue(float h)
{
	glm::vec3 hsv = getHSV();
	hsv.x = h;
	*this = fromHSV(hsv);
}

void Color::setSaturation(float s)
{
	glm::vec3 hsv = getHSV();
	hsv.y = s;
	*this = fromHSV(hsv);
}

void Color::setValue(float v)
{
	glm::vec3 hsv = getHSV();
	hsv.z = v;
	*this = fromHSV(hsv);
}
