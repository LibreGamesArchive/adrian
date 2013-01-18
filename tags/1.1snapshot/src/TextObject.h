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


#ifndef		__TEXTOBJECT_H__
#define		__TEXTOBJECT_H__

#include <GL/glew.h>
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

