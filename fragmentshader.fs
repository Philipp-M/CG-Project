#version 330
#define MAX_LIGHTS 10

uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;

uniform int numPointLights;
uniform struct PointLight
{
	vec3 position;
	vec3 colorIntensity;
	float attenuation;
} allPointLights[MAX_LIGHTS];

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

in vec4 vColor;
in vec3 vNormal;
in vec3 vPosition;
in vec2 vTexCoord;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0, 0, 0, 1.0);
    vec4 texColor = vColor*texture(diffuse, vTexCoord);
    if(texColor.a < 0.5)
        discard;

    vec3 normal = normalize(NormalMatrix * vNormal);

    for(int i = 0; i < numPointLights; ++i)
    {
	    vec3 surfaceToLight = allPointLights[i].position - vPosition;
		float distanceToLight = length(surfaceToLight);

	    float incidence = max(0.0, dot(normal, surfaceToLight) / distanceToLight);
	    float attenuation = 1.0 / (1.0 + allPointLights[i].attenuation * distanceToLight * distanceToLight);
	    FragColor += vec4(attenuation * incidence * allPointLights[i].colorIntensity, 1.0);
	}
	FragColor *= texColor;
	//FragColor += texColor*0.05;
}
