#ifndef		__TTFFONT_H__
#define		__TTFFONT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>

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

