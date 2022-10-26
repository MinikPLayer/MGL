#include "Texture.h"
#include "Util.h"

Texture::Texture(const char* path, int slot)
{
	glActiveTexture(GL_TEXTURE0);
	auto ret = LoadTexture(path, true);

	this->channelsCount = ret.channels;
	this->handle = ret.ID;

	this->slot = slot;

	loaded = true;
}

Texture::Texture(GLuint handleID, int slot)
{
	this->handle = handleID;
	this->slot = slot;

	loaded = true;
}

void Texture::SetActive()
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, handle);
}

Texture::~Texture()
{
	if (loaded)
	{
		LOGW_E("Deleting texture");
		glDeleteTextures(1, &handle);
	}
}
