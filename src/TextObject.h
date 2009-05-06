#ifndef		__TEXTOBJECT_H__
#define		__TEXTOBJECT_H__

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "TTFFont.h"

class TextObject
{
 private:
	char *str;
	int len;
	SDL_Color color;
	bool dirty;

	TTFFont *font;

 public:
	GLuint texid;
	float tx, ty;

	TextObject(TTFFont *font, GLuint tid = INVALID_TEXTURE_ID);
	~TextObject();

	inline int getLen(void)
	{
		return len;
	}

	void setText(const char *s = NULL);
	void setColor(float red, float green, float blue);

};

#endif	/*	__TEXTOBJECT_H__	*/

