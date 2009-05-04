#include "texture.h"

Texture::Texture(const char *imgpath)
{
	strncpy(fn, imgpath, MAX_IMG_PATH_LEN);
	loaded = false;
}

Texture::~Texture()
{
}

int Texture::Load(GLuint texid)
{
	SDL_Surface* surface;

	/* Load the SDL Image into memory */
	surface = IMG_Load(file);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if (texid == INVALID_TEXID)
		glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	SDL_PixelFormat *format = surface->format;

	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  (format->Amask) ? 4 : 3,
					  surface->w, surface->h,
					  (format->Amask) ? GL_RGBA : GL_RGB,
					  GL_UNSIGNED_BYTE, surface->pixels);

	SDL_FreeSurface(surface);

	return 0;
}

int Texture::Unload(void)
{
}


