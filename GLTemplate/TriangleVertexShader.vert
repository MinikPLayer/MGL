#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;

out vec3 vertColor;
out vec2 UV;

uniform float offset;
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//gl_Position = transform * vec4(offset + aPos.x, aPos.y * scale, aPos.z, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vertColor = aColor;
	UV = aUV;
	//vertColor = vec4((aPos.x + 1)/ 2, (aPos.y + 1)/ 2, (aPos.z + 1)/ 2, 1.0f);
}