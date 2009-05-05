#include "texture.h"

Texture::Texture(const char *imgpath)
{
	strncpy(fn, imgpath, MAX_IMG_PATH_LEN);
	fn[MAX_IMG_PATH_LEN - 1] = '\0';
	/* Last 3 chars are extension */
	extension = fn + strlen(fn) - 4;
	loaded = false;
	texid = INVALID_TEXTURE_ID;
}

Texture::~Texture()
{
	if (loaded)
		Unload();
}

int Texture::Load(GLuint tid)
{
	SDL_Surface* surface;
	bool reversed_rgb = false;

	if (loaded) {
		fprintf(stderr, "Warning: Texture object is already loaded: %d\n", texid);
	}

	if (!strcasestr(extension, "tga"))
		reversed_rgb = true;

	/* Load the SDL Image into memory */
	surface = IMG_Load(fn);
	if (surface == NULL) {
		fprintf(stderr, "Warning: Unable to load texture: %s\n", fn);
		return -1;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if (tid == INVALID_TEXTURE_ID)
		glGenTextures(1, &texid);
	else {
		if (glIsTexture(tid) == GL_TRUE) {
			fprintf(stderr, "Warning overwriting existing texture: %d\n", tid);
		}
		texid = tid;
	}

	glBindTexture(GL_TEXTURE_2D, texid);
	SDL_PixelFormat *format = surface->format;

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum colorformat = (reversed_rgb) ?
		((format->Amask) ? GL_RGBA : GL_RGB) :
		(format->Amask) ? GL_BGRA : GL_BGR;

	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  (format->Amask) ? 4 : 3,
					  surface->w, surface->h, colorformat,
					  GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SDL_FreeSurface(surface);
	loaded = true;

	return 0;
}

int Texture::Unload(void)
{
	if (loaded) {
		glDeleteTextures(1, &texid);
		loaded = false;
	}
	return 0;
}

bool Texture::isTextureSame(const char *path)
{
	return (!strcmp(path, fn));
}

