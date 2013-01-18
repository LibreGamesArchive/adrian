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


#ifndef		__TTFFONT_H__
#define		__TTFFONT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
//#include <SDL/SDL_opengl.h>

#include "texture.h"

class TTFFont
{
 private:
	TTF_Font *font;
	char path[256];

 public:
	int height;
	int lineskip;
	int size;
	int style;

	TTFFont(const char *fontpath, int size, int style = TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
	~TTFFont();

	void getTextSize(const char *str, int *w, int *h);
	int createTexture(GLuint texid, const char *str, SDL_Color *color, float *texx, float *texy);

};

#endif	/*	__TTFFONT_H__	*/

