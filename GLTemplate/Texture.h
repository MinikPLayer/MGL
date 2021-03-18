#pragma once

#include <glad/glad.h>

class Texture
{
	bool loaded = false;
	GLuint handle = -1;
	int slot = -1;

public:
	Texture() {}
	Texture(const char* path, int slot);

	void SetActive();

	~Texture();
};