/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2009  Bhanu Chetlapalli, Vamsi Krishna
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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

	/* somehow this is not required on the windows os */
#ifndef WIN32
	if (!strcasestr(extension, "tga"))
		reversed_rgb = true;
#else
	if (strstr(extension, "jpg"))
		reversed_rgb = true;
#endif

	/* Load the SDL Image into memory */
	surface = IMG_Load(fn);
	if (surface == NULL) {
        fprintf(stderr, "Warning: Unable to load texture(%s): %s\n",
                fn, IMG_GetError());
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

    CreateTexFromSurf(surface, texid, reversed_rgb);

	SDL_FreeSurface(surface);
	loaded = true;

	return 0;
}

void Texture::CreateTexFromSurf(SDL_Surface *surface, GLuint texid, bool reversed_rgb)
{
    glBindTexture(GL_TEXTURE_2D, texid);
	SDL_PixelFormat *format = surface->format;

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

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
}

int Texture::Unload(void)
{
	if (loaded) {
		glDeleteTextures(1, &texid);
		loaded = false;
	}
	return 0;
}

/* TODO Replace with a more robust checksum based approach */
bool Texture::isTextureSame(const char *path)
{
	int lp, lf;
	lp = strlen(path) - 1;
	lf = strlen(fn) - 1;

	while (lp >= 0 && lf >= 0) {
		char c = path[lp];
		char c2 = fn[lf];
		if (c >= 'A' && c <= 'Z') 
			c += 'a' - 'A';
		if (c2 >= 'A' && c2 <= 'Z') 
			c2 += 'a' - 'A';

		if (c != c2) {
			return false;
		}

		if (c == '/')
			return true;
		lp--;
		lf--;
	}

	return false;
}

