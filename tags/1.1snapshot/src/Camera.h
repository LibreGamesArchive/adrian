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


#ifndef				__GAME_CAMERA_H__
#define				__GAME_CAMERA_H__

#include <math.h>
#include <GL/glew.h>
/*#include <GL/gl.h>
#include <GL/glu.h>*/

#include <stdio.h>

class Camera {
 public:
	float camx;
	float camy;
	float camz;

	float pointx;
	float pointy;
	float pointz;

	float lookx;
	float looky;
	float lookz;

	float distance;
	float height;
	float angle;
	float initx;
	float initz;

	float yfactor;

 public:
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
	 Camera(void);
	~Camera();

	void Initialize();

    void Update();
    void Move();
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

    void set3DProjection();
    void set2DProjection();

	void Rotate(float a);

	void Zoom(float d);

	int ConvertCoordinates(int x, int y, float &, float &);

	int ScrollOver(float x, float y);

    void GetRayPoints(int x, int y, double *vals);

};

#endif				/*      __GAME_CAMERA_H__       */
