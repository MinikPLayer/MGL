#version 330 core
out vec4 fragColor;

in vec3 vertColor;
in vec2 UV;

uniform sampler2D objTexture;
uniform sampler2D objTexture2;

uniform vec2 uvScale;

uniform float texMix;

void main()
{
	//fragColor = vec4(vertColor, 1.0f); //vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//fragColor = texture(objTexture, UV) * vec4(vertColor, 1.0f);
	fragColor = mix(texture(objTexture, UV), texture(objTexture2, vec2(-UV.x, UV.y) * uvScale), texMix) * vec4(vertColor, 1.0f);
}