#include "PostProcessing.h"
#include "AssetsLoader.h"
#include "Window.h"

void PostProcessing::AttachTexture(Vector2i size)
{
	// Setup and Bind FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Setup TBO
	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)size.x, (int)size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TBO, 0);
	
	// Setup RBO
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, (int)size.x, (int)size.y);

	// Bind RBO to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOGF_E("Error creating PostProcessing framebuffer");

	// Bind back the default FB
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	this->size = size;
}

const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

void PostProcessing::GenerateScreenQuad()
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}

void PostProcessing::ClearFBO()
{
	if (FBO != -1)
	{
		glDeleteRenderbuffers(1, &RBO);
		glDeleteTextures(1, &TBO);
		glDeleteFramebuffers(1, &FBO);
		FBO = -1;
	}
}

void PostProcessing::Clear()
{
	ClearFBO();

	if (quadVAO != -1)
	{
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &quadVAO);

		quadVBO = quadVAO = -1;
	}
}

void PostProcessing::DrawToScreen()
{
	// Bind default FB
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(quadVAO);
	glUseProgram(drawToScreenShader->ID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TBO);
	drawToScreenShader->SetInt("screenTexture", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	
	/*glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glEnable(GL_DEPTH_TEST);*/
}

void PostProcessing::Use()
{
	auto window = Window::GetMainWindow();
	auto sz = window->GetSize();
	if (sz != this->size)
	{
		ClearFBO();
		AttachTexture(sz);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

PostProcessing::PostProcessing(Vector2i size)
{
	drawToScreenShader = AssetsLoader::LoadShader("PP.vert", "PP.frag");

	AttachTexture(size);
	GenerateScreenQuad();
}

PostProcessing::~PostProcessing()
{
	Clear();
}
