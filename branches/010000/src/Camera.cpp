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
