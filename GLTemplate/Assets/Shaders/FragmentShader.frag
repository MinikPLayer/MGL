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

	vec3 GetAmbient()
	{
		return ambient * strength;
	}

	vec3 GetDiffuse()
	{
		return diffuse * strength;
	}

	vec3 GetSpecular()
	{
		return specular * strength;
	}
};

uniform Light light = Light(vec3(1), vec3(1), vec3(1), 1);

out vec4 fragColor;

in vec3 vertColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;
//in vec2 UV;

in vec2 TexCoords;



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
	//vec3 ambient = (texture(material.diffuse, TexCoords)).rgb; // Just for debug purposes
	vec3 ambient = light.GetAmbient() * GetTex2DVec3Value(material.diffuse, TexCoords);
	/*if(material.useTexture)
		ambient = light.ambient * (texture(material.diffuse, TexCoords)).rgb;
	else
		ambient = light.ambient;*/

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);


	// Difuse lightning
	//vec3 diffuse = (texture(material.diffuse, TexCoords)).rgb; // Just for debug purposes
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.GetDiffuse() * GetTex2DVec3Value(material.diffuse, TexCoords);

	/*if(material.useTexture)
		diffuse = diff * (texture(material.diffuse, TexCoords)).rgb * light.diffuse;
	else
		diffuse = diff * light.diffuse;*/


	// Specular lightning
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 255);
	//vec3 specular = specularStrength * (spec * material.specular) * light.specular;
	vec3 specular = specularStrength * (spec * GetTex2DVec3Value(material.specular, TexCoords)) * light.GetSpecular();

	vec3 result = (ambient + diffuse + specular) * vertColor;
	fragColor = vec4(result, 1.0f); 

}