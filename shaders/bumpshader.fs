#version 330
#define MAX_LIGHTS 10

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
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

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

uniform int useNormalMapping;
uniform int useSpecularMapping;

uniform int useDiffuseLightning;
uniform int useSpecularLightning;
uniform int useAmbientLightning;

uniform vec3 specColor;
uniform vec3 difColor;
uniform float shininess;

in vec3 vNormal;
in vec3 vTangent;
in vec3 vPosition;
in vec3 vPosView;
in vec2 vTexCoord;

out vec4 FragColor;

vec3 calcNormal()
{
    vec3 normal = normalize(normalMatrix * vNormal);
    vec3 tangent = normalize(normalMatrix * vTangent);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    vec3 biTangent = cross(tangent, normal);
    vec3 nMapNormal = (2.0 *  texture(normalTex, vTexCoord).xyz - vec3(1.0, 1.0, 1.0));
    nMapNormal = vec3(nMapNormal.x, -nMapNormal.y, nMapNormal.z);
    mat3 TBN = mat3(tangent, biTangent, normal);
    return normalize(TBN * nMapNormal);
}
void main()
{
    vec4 texColor = vec4(difColor, 1.0) * texture(diffuseTex, vTexCoord);
    if(texColor.a < 0.5)
        discard;
	FragColor = vec4(0, 0, 0, 1.0);
    vec3 normal;
	if(useNormalMapping == 1)
		normal = calcNormal();
    else
        normal = normalize(normalMatrix * vNormal);
	float shine = shininess;
    if(useSpecularMapping == 1)
        shine *= texture(specularTex, vTexCoord).r*10;

    for(int i = 0; i < numPointLights; ++i)
    {
	    vec3 posToLight = allPointLights[i].position - vPosition;
		float disToLight = length(posToLight);
		posToLight = normalize(posToLight);
		// ambient lightning
		vec3 ambientLight = vec3(0.0,0.0,0.0);
		if(useAmbientLightning == 1)
			ambientLight = allPointLights[i].ambient * texColor.rgb;
		// diffuse lightning
	    float cosNorm = max(0.0, dot(normal, posToLight));

	    vec3 difLight = vec3(0.0,0.0,0.0);
	    if(useDiffuseLightning == 1)
	        difLight = cosNorm * texColor.rgb;

	    // specular lightning
	    vec3 specLight = vec3(0, 0, 0);
	    if(useSpecularLightning == 1)
	    {
	        vec3 posToCamera = normalize(-cameraPosition - vPosition);
			vec3 refVector = reflect(-posToLight, normal);
	        float cosRefl = max(0.0, dot(posToCamera,refVector));
			float specCof = pow(cosRefl, 0.01+shine);
			specLight = specCof * specColor ;
	    }

	    float attenuation = 1.0 / (1.0 + allPointLights[i].attenuation * disToLight * disToLight);
	    FragColor += vec4(attenuation * allPointLights[i].colorIntensity * (difLight + 1*specLight) + allPointLights[i].colorIntensity * ambientLight , 0.0);
	}
}
