#version 330 core
out vec4 fragColor;

in vec3 vertColor;

void main()
{
	fragColor = vec4(vertColor, 1.0f); //vec4(1.0f, 0.5f, 0.2f, 1.0f);
}