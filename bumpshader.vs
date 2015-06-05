#version 330

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 MVPMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec2 texCoord;

out vec3 vTangent;
out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vPosition;
out vec3 vPosView;

void main()
{

	gl_Position = MVPMatrix * vec4(position.x, position.y, position.z, 1.0);
	vTangent = tangent;
	vPosition = position;
	vPosView = vec3(modelViewMatrix * vec4(position, 1.0));
	vNormal = normal;
	vTexCoord = texCoord;
}
