#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 _resolution_;

out vec3 pos;
out vec2 fsun;

uniform float time = 0.0;

const vec2 data[6] = vec2[](
vec2(-1.0,  1.0), vec2(-1.0, -1.0),
vec2( 1.0,  1.0), vec2( 1.0, -1.0),
vec2(-1.0, 1.0), vec2(-1.0, -1.0));

void main()
{
	gl_Position = vec4(aPos, 1.0);
	pos = transpose(mat3(view)) * (inverse(projection) * gl_Position).xyz;
	//fsun = vec2(sin(time * 0.1), cos(time * 0.1)) * _resolution_;
	fsun = vec2(800, 600);
}