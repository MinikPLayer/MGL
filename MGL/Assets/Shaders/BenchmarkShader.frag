#version 440

uniform vec3 cubeColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(cubeColor, 1.0f);
}