#pragma once

#include "glad/glad.h"
#include "Shader.h"

class Framebuffer {
protected:

	GLuint FBO = -1;

public:
	Framebuffer();

	~Framebuffer();
};