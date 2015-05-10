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
    vec4 texel = vColor*texture(diffuse, vTexCoord);
    if(texel.a < 0.5)
        discard;
	FragColor = texel;
}
