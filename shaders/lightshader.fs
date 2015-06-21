#version 330

uniform vec3 difColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(normalize(difColor), 1.0);
	float maxVal = max(max(FragColor.x, FragColor.y), FragColor.z);
	FragColor /= maxVal;
	FragColor.a = 1.0;
}
