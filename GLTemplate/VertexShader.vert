#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aColor;

out vec2 TexCoords;

out vec3 vertColor;
out vec3 Normal;
out vec3 FragPos;
out vec3 lightPosition;
//out vec2 UV;

uniform vec3 lightPos;

uniform float offset;
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = transform * vec4(offset + aPos.x, aPos.y * scale, aPos.z, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertColor = aColor;
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	lightPosition = vec3(view * vec4(lightPos, 1.0));
	TexCoords = aTexCoords;
	//UV = aUV;
	//vertColor = vec4((aPos.x + 1)/ 2, (aPos.y + 1)/ 2, (aPos.z + 1)/ 2, 1.0f);
}