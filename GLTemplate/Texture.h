#pragma once

#include <glad/glad.h>
#include "Asset.h"

class Texture : public Asset
{
	bool loaded = false;
	GLuint handle = -1;
	int slot = -1;

public:
	Texture() {}
	Texture(const char* path, int slot);

	int GetSlot() { return this->slot; }
	void SetActive();

	~Texture();
};