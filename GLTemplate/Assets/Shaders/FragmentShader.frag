#version 330 core

// Union between vec3 and texture
struct Tex2DVec3
{
	bool useTex;
	sampler2D tex;
	vec3 value;
};

struct Material {
	//bool useDiffTex;
	//sampler2D diffuse;
    //vec3 specular;
	Tex2DVec3 diffuse;
	Tex2DVec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float strength;

	float constant;
	float linear;
	float quadratic;
};

vec3 GetLightAmbient(Light light)
{
	return light.ambient * light.strength;
}

vec3 GetLightDiffuse(Light light)
{
	return light.diffuse * light.strength;
}

vec3 GetLightSpecular(Light light)
{
	return light.specular * light.strength;
}


uniform Light light = Light(vec3(1), vec3(1), vec3(1), 1, 1, 0.09f, 0.032f);

out vec4 fragColor;

in vec3 vertColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;
//in vec2 UV;

in vec2 TexCoords;
uniform vec2 texScale = vec2(1,1);

float specularStrength = 0.5;


vec3 GetTex2DVec3Value(Tex2DVec3 value, vec2 coords)
{
	if(value.useTex)
		return texture(value.tex, coords).rgb;
	else
		return value.value;
}


void main()
{

	vec3 ambient = GetLightAmbient(light) * GetTex2DVec3Value(material.diffuse, TexCoords / texScale);

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);


	// Difuse lightning
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * GetLightDiffuse(light) * GetTex2DVec3Value(material.diffuse, TexCoords / texScale);


	// Specular lightning
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 255);
	vec3 specular = specularStrength * (spec * GetTex2DVec3Value(material.specular, TexCoords / texScale)) * GetLightSpecular(light);

	vec3 result = (ambient + diffuse + specular) * vertColor;
	fragColor = vec4(result, 1.0f); 

}