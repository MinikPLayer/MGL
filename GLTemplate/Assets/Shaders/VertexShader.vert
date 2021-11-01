#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aColor;

out vec2 TexCoords;

out vec3 vertColor;
out vec3 Normal;
out vec3 FragPos;
//out vec2 UV;
out vec4 FragPosLightSpace;


uniform float offset;
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	vertColor = aColor;
	Normal = mat3(transpose(inverse(model))) * aNormal;  

	TexCoords = aTexCoords;

	FragPos = vec3(model * vec4(aPos, 1.0));

	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

	gl_Position = projection * view * vec4(FragPos, 1.0);

}