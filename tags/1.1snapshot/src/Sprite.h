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


#ifndef		__SPRITE_H__
#define		__SPRITE_H__

#include <GL/glew.h>
//#include <GL/glu.h>

class Sprite
{
 private:
	float centx, centz;
	GLint texid;
 public:
    static bool is_bilboard;
	Sprite(GLint texid, float x, float y);
	~Sprite();

	void Render(void);
	
};

#endif	/*	__SPRITE_H__	*/

