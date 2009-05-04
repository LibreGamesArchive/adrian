#include "texture.h"

Texture::Texture(const char *imgpath)
{
	strncpy(fn, imgpath, MAX_IMG_PATH_LEN);
	loaded = false;
}

Texture::~Texture()
{
}

int Texture::Load(void)
{
	/* Load the SDL Image into memory */
}

int Texture::Unload(void)
{
}


