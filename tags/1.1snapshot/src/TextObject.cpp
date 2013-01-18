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


#include "TextObject.h"

TextObject::TextObject(TTFFont *font, GLuint tid)
{
	str = NULL;
	len = 0;

	texid = tid;
	if (texid == INVALID_TEXTURE_ID)
		glGenTextures(1, &texid);

	this->font = font;
	setColor(1.0, 1.0, 1.0);
	dirty = true;

	tx = ty = 1.0;
}

TextObject::~TextObject()
{
	if (str)
		free(str);
	if (glIsTexture(texid))
		glDeleteTextures(1, &texid);
}

void TextObject::setColor(float red, float green, float blue)
{
	color.r = (int)(red   * 255.0);
	color.g = (int)(green * 255.0);
	color.b = (int)(blue  * 255.0);
	dirty = true;
}

void TextObject::setText(const char *s)
{
	if (s) {
		/* Same text - do nothing */
		if (str && !strcmp(str, s) && !dirty)
			return;

		if (str)
			free(str);

		len = strlen(s) + 1;
		str = (char *)malloc(len);
		strcpy(str, s);

		font->createTexture(texid, str, &color, &tx, &ty);
		dirty = false;
	} else {
		if (str)
			free(str);

		if (glIsTexture(texid))
			glDeleteTextures(1, &texid);

		str = NULL;
		len = 0;
	}
}

