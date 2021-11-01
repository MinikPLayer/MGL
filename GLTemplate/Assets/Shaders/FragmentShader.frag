#version 330 core

// Union between vec3 and texture
struct Tex2DVec3
{
	bool useTex;
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


vec3 GetTex2DVec3Value(Tex2DVec3 value, vec2 coords)
{
	if(value.useTex)
		return texture(value.tex, coords).rgb;
	else
		return value.value;
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
    return (ambient + diffuse + specular);
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
    return (ambient + diffuse + specular);
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