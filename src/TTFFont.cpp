#include "TTFFont.h"

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

	printf("Creating TextTexture for %s at TexID:%d\n", str, texid);

	txtSDLSurface = TTF_RenderText_Solid(font, str, *color);
	if (txtSDLSurface == NULL) {
		printf("TTF_RenderText_Blended: %s\n", SDL_GetError());
		exit(-1);
	}
	
	int w,h;

	/* Convert the rendered text to a known format */
	w = nextpoweroftwo(txtSDLSurface->w);
	h = nextpoweroftwo(txtSDLSurface->h);
	
	*texx = ((float)txtSDLSurface->w) / w;
	*texy = ((float)txtSDLSurface->h) / h;

	intermediary = SDL_CreateRGBSurface(0, w, h, 32,
		    0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    SDL_BlitSurface(txtSDLSurface, 0, intermediary, 0);

	/* Tell GL about our new texture */
	glBindTexture(GL_TEXTURE_2D, texid);

glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, 
				 GL_UNSIGNED_BYTE, intermediary->pixels );

	/* GL_NEAREST looks horrible, if scaled... */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	SDL_FreeSurface(txtSDLSurface);
	SDL_FreeSurface(intermediary);

	return 0;
}
