/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2003  Bhanu Chetlapalli, Vamsi Krishna, Swamy Suman
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


#ifndef __PANEL_H__
#define __PANEL_H__

#include "TTFFont.h"
#include "TextObject.h"

class Panel {
 private:
	TTFFont *font;

	TextObject *fps;
	TextObject *gameOver;
	TextObject *gameOver2;
	TextObject *gameRes;

 public:
	int textureId;
	bool show_fps;
	 Panel(GLuint);
	~Panel();
	void Render();
	void drawBot();
	void drawTips(const char *, const char *);

	void RenderTextObject(TextObject*, float, float, float w = 15, float h = 20);
};

#endif				// __PANEL_H__
