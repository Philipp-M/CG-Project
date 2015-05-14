#version 330
#define MAX_LIGHTS 10

uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPosition;

uniform int numPointLights;

uniform struct PointLight
{
	vec3 position;
	vec3 colorIntensity;
	float attenuation;
	float ambient;
} allPointLights[MAX_LIGHTS];

uniform sampler2D diffuse;
uniform sampler2D specular;
uniform sampler2D normal;

uniform vec3 specColor;
uniform float shininess;

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

    vec3 normal = normalize(normalMatrix * vNormal);
	vec3 pos = vec3(modelMatrix * vec4(vPosition, 1));

    for(int i = 0; i < numPointLights; ++i)
    {
	    vec3 posToLight = allPointLights[i].position - pos;
		float disToLight = length(posToLight);
		posToLight = normalize(posToLight);
		// ambient lightning
		vec3 ambientLight = allPointLights[i].ambient * texColor.rgb;
		// diffuse lightning
	    float cosNorm = max(0.0, dot(normal, posToLight));

	    vec3 difLight = cosNorm * texColor.rgb;

	    // specular lightning
	    vec3 specLight = vec3(0, 0, 0);
	    if(cosNorm > 0.0)
	    {
	        vec3 posToCamera = normalize(cameraPosition - pos);
			vec3 refVector = reflect(-posToLight, normal);
	        float cosRefl = max(0.0, dot(posToCamera, refVector));
			float specCof = pow(cosRefl, shininess);
			specLight = specCof * specColor ;
	    }

	    float attenuation = 1.0 / (1.0 + allPointLights[i].attenuation * disToLight * disToLight);
	    FragColor += vec4(attenuation * allPointLights[i].colorIntensity * (ambientLight + difLight + specLight), 0.0);
	}
}
