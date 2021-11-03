#pragma once

#include <glad/glad.h>
#include "Asset.h"
#include <string>

using namespace std;

class Texture : public Asset
{
	bool loaded = false;
	GLuint handle = -1;
	int slot = -1;
	int channelsCount = 0;

public:
	int GetChannelsCount() { return this->channelsCount; }

	Texture() {}
	Texture(const char* path, int slot);
	Texture(GLuint handleID, int slot);

	int GetSlot() { return this->slot; }
	void SetActive();

	~Texture();
};