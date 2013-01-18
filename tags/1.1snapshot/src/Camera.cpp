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


#include "Camera.h"
#include "globals.h"
#include "config.h"
#include "Game.h"

Camera::Camera(void)
{
//	Initialize();
}

Camera::~Camera()
{
}

void Camera::Initialize(void)
{
	distance = 50;
	height = 30;
	angle = START_ANGLE;

	pointx = 0;
	pointy = 0;
	pointz = 0;

	lookx = 0;
	looky = 1;
	lookz = 0;

	initx = game->hero->curx;
	initz = game->hero->cury;
	
	camy = height;
	camx = distance * sin(angle) + initx;
	camz = distance * cos(angle) + initz;

	yfactor = -sqrt(1 + (distance * distance) / (height * height));

	Move();
}

void Camera::Rotate(float a)
{
	angle = a;
	camx = distance * sin(angle) + initx;
	camz = distance * cos(angle) + initz;

	pointx = initx;
	pointz = initz;
}

void Camera::Move(void)
{

	camx = distance * sin(angle) + initx;
	camz = distance * cos(angle) + initz;

	pointx = initx;
	pointz = initz;
}

void Camera::MoveUp(void)
{
	float tmpx, tmpy;
	ConvertCoordinates((hres/2.0), (vres/2.0), tmpx, tmpy);
	if (tmpy > game->map->breadth / 2)
		return;

	initx += sin(angle);
	initz += cos(angle);
	Move();
}

void Camera::MoveDown(void)
{
	float tmpx, tmpy;
	ConvertCoordinates((hres/2.0), (vres/2.0), tmpx, tmpy);
	if (tmpy < -game->map->breadth / 2)
		return;

	initx -= sin(angle);
	initz -= cos(angle);
	Move();
}

void Camera::MoveLeft(void)
{
	float tmpx, tmpy;
	ConvertCoordinates((hres/2.0), (vres/2.0), tmpx, tmpy);
	if (tmpx > game->map->length / 2)
		return;

	initx += cos(angle);
	initz -= sin(angle);
	Move();
}

void Camera::MoveRight(void)
{
	float tmpx, tmpy;
	ConvertCoordinates((hres/2.0), (vres/2.0), tmpx, tmpy);
	if (tmpx < -game->map->length / 2)
		return;

	initx -= cos(angle);
	initz += sin(angle);
	Move();
}

void Camera::set2DProjection()
{
    glViewport(0, 0, hres, vres);
	glDisable(GL_DEPTH_TEST);
	//glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, hres, 0, vres, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void Camera::set3DProjection()
{
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, hres, vres);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(hres/2.0), (hres/2.0), -(vres/2.0), (vres/2.0), -hres, hres);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
	gluLookAt(camx, camy, camz, pointx, pointy, pointz, lookx,
			  looky, lookz);
    //get the projection matrix		
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	//get the modelview matrix		
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	//get the viewport		
    glGetIntegerv( GL_VIEWPORT, viewport );
}

void Camera::Update(void) 
{
	glLoadIdentity();
	gluLookAt(camx, camy, camz, pointx, pointy, pointz, lookx,
			  looky, lookz);
    //gluLookAt(0, 50, 100, 0, 0, 0, 0, 1, 0);
    //get the projection matrix		
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
	//get the modelview matrix		
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	//get the viewport		
    glGetIntegerv( GL_VIEWPORT, viewport );
} 

void Camera::GetRayPoints(int x, int y, double *vals)
{
    //vals should be a pointer to 6 vals. ray start, ray end.
    gluUnProject(x, viewport[3]-y, 0, modelview, projection, viewport, &vals[0], &vals[1], &vals[2]);
    gluUnProject(x, viewport[3]-y, 1, modelview, projection, viewport, &vals[3], &vals[4], &vals[5]);
}

void Camera::Zoom(float d)
{
	camy = (d / distance) * camy;

	distance = d;

	camx = distance * sin(angle) + initx;
	camz = distance * cos(angle) + initz;
}

int Camera::ConvertCoordinates(int x, int y, float &x3, float &y3)
{
	float newx = x - (hres/2.0);
	float newy = yfactor * ((vres/2.0) - y);

	x3 = newx * cos(angle) + newy * sin(angle);
	y3 = newy * cos(angle) - newx * sin(angle);

	x3 += pointx;
	y3 += pointz;

	return 0;
}

int Camera::ScrollOver(float x, float y)
{
	initx = x;
	initz = y;
	Move();
	return 0;
}
