#version 330

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 FragColor;

void main()
{
	FragColor = 0.5*(vColor+texture(diffuse, vTexCoord));
}
