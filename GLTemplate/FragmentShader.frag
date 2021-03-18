#version 330 core
struct Material {
	bool useTexture;
	sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

out vec4 fragColor;

in vec3 vertColor;
in vec3 Normal;
in vec3 FragPos;
in vec3 lightPosition;
//in vec2 UV;

in vec2 TexCoords;

uniform vec3 lightColor;


float specularStrength = 0.5;

void main()
{
	//vec3 ambient = (texture(material.diffuse, TexCoords)).rgb; // Just for debug purposes
	vec3 ambient;
	if(material.useTexture)
		ambient = light.ambient * (texture(material.diffuse, TexCoords)).rgb;
	else
		ambient = light.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPos);


	// Difuse lightning
	//vec3 diffuse = (texture(material.diffuse, TexCoords)).rgb; // Just for debug purposes
	vec3 diffuse;
	float diff = max(dot(norm, lightDir), 0.0);

	if(material.useTexture)
		diffuse = diff * (texture(material.diffuse, TexCoords)).rgb * light.diffuse;
	else
		diffuse = diff * light.diffuse;


	// Specular lightning
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 255);
	vec3 specular = specularStrength * (spec * material.specular) * light.specular;

	vec3 result = (ambient + diffuse + specular) * vertColor;
	fragColor = vec4(result, 1.0f); 

}