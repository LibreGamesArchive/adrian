#include "Camera.h"
#include "globals.h"

Camera::Camera(void)
{
	Initialize();
}

Camera::~Camera()
{
}

void Camera::Initialize(void)
{
	distance = 50;
	height = 30;
	angle = 0;

	camx = 0;
	camy = height;
	camz = distance;

	pointx = 0;
	pointy = 0;
	pointz = 0;

	lookx = 0;
	looky = 1;
	lookz = 0;

	initx = hero->curx;
	initz = hero->cury;

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

	Update();
}

void Camera::Move(void)
{

	camx = distance * sin(angle) + initx;
	camz = distance * cos(angle) + initz;

	pointx = initx;
	pointz = initz;

	Update();
}

void Camera::MoveUp(void)
{
	float tmpx, tmpy;
	ConvertCoordinates(320, 240, tmpx, tmpy);
	if (tmpy > map->breadth / 2)
		return;

	initx += sin(angle);
	initz += cos(angle);
	Move();
}

void Camera::MoveDown(void)
{
	float tmpx, tmpy;
	ConvertCoordinates(320, 240, tmpx, tmpy);
//      printf("tmpy = %f\n",tmpy);
	if (tmpy < -map->breadth / 2)
		return;

	initx -= sin(angle);
	initz -= cos(angle);
	Move();
}

void Camera::MoveLeft(void)
{
	float tmpx, tmpy;
	ConvertCoordinates(320, 240, tmpx, tmpy);
	if (tmpx > map->length / 2)
		return;

	initx += cos(angle);
	initz -= sin(angle);
	Move();
}

void Camera::MoveRight(void)
{
	float tmpx, tmpy;
	ConvertCoordinates(320, 240, tmpx, tmpy);
	if (tmpx < -map->length / 2)
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

	Update();

}

int Camera::ConvertCoordinates(int x, int y, float &x3, float &y3)
{
	float newx = x - 320;
	float newy = yfactor * (240 - y);

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
