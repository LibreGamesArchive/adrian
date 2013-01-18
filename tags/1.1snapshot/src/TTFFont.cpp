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


#include "TTFFont.h"
#include "texture.h"

TTFFont::TTFFont(const char *fontpath, int size, int style)
{
	if(!TTF_WasInit() && TTF_Init() == -1) {
		printf("TTF_Init failed: %s\n", TTF_GetError());
		exit(1);
	}

	this->size = size;
	this->style = style;

	font = TTF_OpenFont(fontpath, size);
	if(!font) {
		printf("TTF_OpenFont(%s): %s\n", fontpath, TTF_GetError());
		exit(1);
	}

	TTF_SetFontStyle(font, style);

	height = TTF_FontHeight(font);
	lineskip = TTF_FontLineSkip(font);
}

TTFFont::~TTFFont()
{
	TTF_CloseFont(font);
}

void TTFFont::getTextSize(const char *str, int *w, int *h)
{
	TTF_SizeText(font, str, w, h);
}

int nextpoweroftwo(int x)
{
	int i;
	for (i = 0; x >> i; i++);
	return 0x1 << i;
}

int TTFFont::createTexture(GLuint texid, const char *str, SDL_Color *color, float *texx, float *texy)
{
	SDL_Surface *txtSDLSurface;
	SDL_Surface *intermediary;

	//printf("Creating TextTexture for %s at TexID:%d\n", str, texid);

	txtSDLSurface = TTF_RenderText_Solid(font, str, *color);
	if (txtSDLSurface == NULL) {
		printf("TTF_RenderText_Blended: %s\n", SDL_GetError());
		exit(-1);
	}
	
	int w,h;

	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(txtSDLSurface->w);
	h = nextpoweroftwo(txtSDLSurface->h);
	
	*texx = ((float)txtSDLSurface->w + 1.0) / w;
	*texy = ((float)txtSDLSurface->h + 1.0) / h;

	intermediary = SDL_CreateRGBSurface(0, w, h, 32,
		    0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_BlitSurface(txtSDLSurface, 0, intermediary, 0);

	/* Tell GL about our new texture */
    Texture::CreateTexFromSurf(intermediary, texid);

	SDL_FreeSurface(txtSDLSurface);
	SDL_FreeSurface(intermediary);

	return 0;
}
