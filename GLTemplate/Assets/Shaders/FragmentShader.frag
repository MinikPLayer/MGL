#version 330 core

// Union between vec3 and texture
struct Tex2DVec3
{
	bool useTex;
	bool useOnlyRed;
	sampler2D tex;
	vec3 value;
};

struct Material {
	Tex2DVec3 diffuse;
	Tex2DVec3 specular;
    float shininess;
}; 

struct DirLight {
	vec3 direction;
	float strength;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float strength;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;
uniform vec3 shadowPos;

out vec4 fragColor;
in vec3 vertColor;
in vec3 Normal;
in vec3 FragPos;
//in vec2 UV;

in vec2 TexCoords;
uniform vec3 viewPos;
uniform vec2 texScale = vec2(1,1);

float specularStrength = 0.5;

  
uniform Material material;
uniform DirLight dirLight;

uniform int pointLightsCount = 0;
const int MAX_LIGHTS_COUNT = 4;
uniform PointLight pointLights[MAX_LIGHTS_COUNT];

float ShadowCalculation2(vec4 lightSpace)
{
	vec3 projCoords = lightSpace.xyz / lightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	//float bias = max(0.1 * (1.0 - dot(Normal, -dirLight.direction)), 0.0005);  
	vec3 lightDir = normalize(-dirLight.direction - FragPos.xyz);

	float cosTheta = clamp(dot(Normal, lightDir), 0.0, 1.0);

	//float bias = clamp(0.000005 * tan(acos(cosTheta)), 0.0001, 0.005);
	//float bias = clamp(0.000005 * tan(acos(cosTheta)), 0.0001, 0.005);
	float bias = max(0.001 * (1.0 - dot(Normal, lightDir)), 0.001);  
	//float bias = 0.0002;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

float ShadowCalculation(vec4 lightSource)
{
	vec3 L = normalize(-dirLight.direction - FragPos);
	vec3 N = normalize(Normal);

	float NdL = max(dot(N,L), 0.0);

	// perform perspective divide
    vec3 projCoords = lightSource.xyz / lightSource.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

	if(projCoords.z > 1.0)
		return 0.0;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow

    float bias = max(0.025 * (1.0 - NdL), 0.001);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 GetTex2DVec3Value(Tex2DVec3 value, vec2 coords)
{
	if(value.useTex)
		if(value.useOnlyRed)
		{
			float val = texture(value.tex, coords).r;
			return vec3(val, val, val);
		}
		else
			return texture(value.tex, coords).rgb;
	else
		return value.value;
	//return texture(shadowMap, coords).rgb;
}

vec3 CalcLight_Dir(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 255);
    // combine results
    vec3 ambient = light.ambient * light.strength * GetTex2DVec3Value(material.diffuse, TexCoords);
    vec3 diffuse = light.diffuse * light.strength * diff * GetTex2DVec3Value(material.diffuse, TexCoords);
    vec3 specular = light.specular * light.strength * spec * GetTex2DVec3Value(material.specular, TexCoords);
    
	float shadow = ShadowCalculation(FragPosLightSpace);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
	//return (ambient + diffuse + specular);
}

vec3 CalcLight_Point(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading - Blinn Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 255);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient * light.strength * GetTex2DVec3Value(material.diffuse, TexCoords);
    vec3 diffuse  = light.diffuse  * light.strength * diff * GetTex2DVec3Value(material.diffuse, TexCoords);
    vec3 specular = light.specular * light.strength * spec * GetTex2DVec3Value(material.specular, TexCoords);
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

	float shadow = ShadowCalculation(FragPosLightSpace);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
	//return (ambient + diffuse + specular);
}

void main()
{
	// Check for alpha discard
	if(material.diffuse.useTex) {
		vec4 texColor = texture(material.diffuse.tex, TexCoords);
		if(texColor.a < 0.1)
			discard;
	}

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = CalcLight_Dir(dirLight, norm, viewDir);
	for(int i = 0;i<pointLightsCount;i++) {
		result += CalcLight_Point(pointLights[i], norm, FragPos, viewDir);
	}

	fragColor = vec4(result, 1.0);
	//fragColor = vec4(0,1,0,1);
}