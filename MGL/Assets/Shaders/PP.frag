#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float spec = 1.2;

void main()
{
	vec4 value = texture(screenTexture, TexCoords);
	value = vec4(pow(value.x, spec), pow(value.y, spec), pow(value.z, spec), 1.0);

	FragColor = value;
}