#include "Framebuffer.h"
#include "Shader.h"
#include "PlaneMesh.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &FBO);
}

Framebuffer::~Framebuffer()
{
	if (FBO != -1) {
		glDeleteFramebuffers(1, &FBO);
	}
}
