#version 330

uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texCoord;

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vPosition;

void main()
{

	gl_Position = ViewMatrix*ModelMatrix*vec4(position.x, position.y, position.z, 1.0);
	vColor = vec4(color.r, color.g, color.b, 1.0);
	vPosition = position;
	vNormal = normal;
	vTexCoord = texCoord;
}
