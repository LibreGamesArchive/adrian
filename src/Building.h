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


#ifndef				__GAME_BUILDING_H__
#define				__GAME_BUILDING_H__

#include <stdio.h>
#include <GL/glew.h>
/*#include <GL/gl.h>
#include <GL/glu.h>*/

class Building {
 private:
	char filepath[256];
 public:
	int buildingID;
	int buildingType;

	GLuint dlist;

	float x1;
	float x2;
	float y1;
	float y2;
	float h;

	float bx1;
	float bx2;
	float by1;
	float by2;

	int textureID;

 public:
	 Building(void);
	~Building();

	int Load(const char *fn);

	bool isInside(float x, float y);
	void setType(int);
	void Render(void);
	void Dump(void);

};

#endif				/*      __GAME_BUILDING_H__     */
