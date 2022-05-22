#pragma once

#include "Framebuffer.h"
#include "Util.h"
#include "Shader.h"
#include "PlaneMesh.h"

class PostProcessing
{
	shared_ptr<Shader> drawToScreenShader;
	GLuint quadVBO = -1;
	GLuint quadVAO = -1;

	/// <summary>
	/// Frame Buffer Object
	/// </summary>
	GLuint FBO = -1;

	/// <summary>
	/// Texture Buffer Object
	/// </summary>
	GLuint TBO = -1;

	/// <summary>
	/// Render Buffer Object
	/// </summary>
	GLuint RBO = -1;

	Vector2i size;

	void AttachTexture(Vector2i size);
	void GenerateScreenQuad();

	void Clear();
	void ClearFBO();

public:
	void DrawToScreen();
	void Use();

	PostProcessing(Vector2i size);
	~PostProcessing();
};