#include "Texture.h"
#include "Util.h"

Texture::Texture(const char* path, int slot)
{
	glActiveTexture(GL_TEXTURE0);
	handle = LoadTexture(path);
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
