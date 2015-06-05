#version 330

uniform mat4 MVPMatrix;

layout (location = 0) in vec3 position;

void main()
{
	gl_Position = MVPMatrix * vec4(position.x, position.y, position.z, 1.0);
}
