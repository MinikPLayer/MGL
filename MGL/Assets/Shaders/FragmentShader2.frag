#version 330 core
out vec4 fragColor;

in vec2 TexCoords;

struct Material
{
	sampler2D diffuse;
};

uniform Material material;


void main()
{
	fragColor = vec4((texture(material.diffuse, TexCoords)).rgb, 1.0); //vec4(1.0f, 0.5f, 0.2f, 1.0f);
}