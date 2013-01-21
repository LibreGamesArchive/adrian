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


#include "Hero.h"
#include "globals.h"
#include "Game.h"

#define		PI		3.141

Hero::Hero(float x, float y, float f, float s, GLuint fovTexID, GLuint panelTexID)
{
	Initialize(x, y, f, s, fovTexID, panelTexID);
}

Hero::~Hero()
{
	basemodel->Unload();
}

void Hero::Initialize(float x, float y, float f, float s, GLuint fovTexID, GLuint panelTexID)
{
	basemodel = new MD2;

	basemodel->Load(GAME_DATA_PATH"/models/dynamic/hero/hero.md2");

	this->x = curx = x;
	this->z = cury = y;
	this->y = MAP_MODEL_HEIGHT_Y;
	destx1 = destx = 0;
	desty1 = desty = 0;
	speed = s;
	facingAngle = f;
	selected = true;
	gunselected = false;
	Alive = true;
	Stand();
	textureID = 2;

	{
		fov = 150;
		fovAngle = 60;
		fovInterval = (PI / 2.0 - PI * fovAngle / 180.0) / 10.0;
		fovEnd = PI * fovAngle / 180.0;
		fovStart = 2 * fovEnd;
		fovID = fovTexID;
	}
	PanelTexId = panelTexID;
}

void Hero::Run(float dx, float dy, float perfectx, float perfecty)
{
	Compute(dx, dy, perfectx, perfecty);

	if (status != HERO_RUNNING) {
		status = HERO_RUNNING;
		basemodel->setAnimation(ANIMTYPE_RUN);
	}
}

void Hero::Stand(void)
{
	status = HERO_STANDING;
	basemodel->setAnimation(ANIMTYPE_STAND);
}

void Hero::Attack(void)
{
	status = HERO_ATTACKING;
	AttackFrameCount = basemodel->getNumFrames(ANIMTYPE_ATTACK);
	//usleep(500000);
	basemodel->setAnimation(ANIMTYPE_ATTACK);
}

void Hero::Death(void)
{
	status = HERO_DEAD;
	DeathFrameCount = basemodel->getNumFrames(ANIMTYPE_DEATH);
	basemodel->setAnimation(ANIMTYPE_DEATH);
}

int Hero::NextMove(void)
{
	switch (status) {
	case HERO_STANDING:
		return 0;
	case HERO_ATTACKING:
		if (AttackFrameCount == 0)
			Stand();
		return 0;
	case HERO_DEAD:
//		if (DeathFrameCount == 0)
//		TODO Never worked! :)
//			Alive == false;
		return 0;
	case HERO_RUNNING:
		{
			if (curx >= destx - 10 && curx <= destx + 10) {
				if (cury <= desty + speed
				    && cury >= desty - speed) {
					x = curx = destx;
					z = cury = desty;
					if ((destx == destx1)
					    && (desty == desty1)) {
						Stand();
						return 0;
					} else {
						destx = destx1;
						desty = desty1;
						Compute(destx, desty, destx,
							desty);
					}
				}
			}

			float newx, newy;

			if (movingright)
				newx = curx + speed * costheta;
			else
				newx = curx - speed * costheta;
			if (movingup)
				newy = cury + speed * sintheta;
			else
				newy = cury - speed * sintheta;

			int newbx, newby;

			newbx =
			    (int)(game->map->length / 2.0 + newx) / game->map->blocksize +
			    1;
			newby =
			    (int)(game->map->breadth / 2.0 + newy) / game->map->blocksize +
			    1;

			bool collision = false;
			for (int i = 0; i < game->map->no_of_buildings; i++) {
				if ((((newbx >= game->map->buildings[i].bx1)
				      && (newbx <= game->map->buildings[i].bx2))
				     && ((newby == game->map->buildings[i].by1)
					 || (newby == game->map->buildings[i].by2)))
				    ||
				    (((newby >= game->map->buildings[i].by1)
				      && (newby <= game->map->buildings[i].by2))
				     && ((newbx == game->map->buildings[i].bx1)
					 || (newbx == game->map->buildings[i].bx2)))
				    ) {
					if (game->cheat_code_nowalls_enabled)
						break;
			
					float tempx, tempy;
					game->block_convert(tempx, tempy, curx, cury);
					destx = x = curx = tempx;
					desty = z = cury = tempy;
					Stand();

					collision = true;

					break;
				}
			}

			if (!collision) {
				curx = newx;
				cury = newy;
			}
			x = curx;
			z = cury;
		}
	}
	return 0;
}

bool Hero::CheckCurWithBuildings(float &x, float &y)
{
	return true;

	bool collision = false;
	for (int i = 0; i < game->map->no_of_buildings; i++) {
		if (game->map->buildings[i].isInside(x, y)) {
			collision = true;
		}
	}
	if (collision)
		return 1;
	else
		return 0;
}

bool Hero::CheckCurWithWalls(float &x, float &y)
{
	if (((x < game->map->length / 2.0) && (x > -game->map->length / 2.0))
	    && ((y < game->map->breadth / 2.0) && (y > -game->map->breadth / 2.0)))
		return false;

	return true;
}

int Hero::BuildingOnPath()
{
	return 0;

}

int Hero::Compute(float &dx, float &dy, float perfectx, float perfecty)
{

	float m = (dy - cury) / (dx - curx);
	float newx = dx;
	float newy = dy;

	{			// loop for calculating fov and dx & dy

		if (dx == curx) {
			costheta = 0;
			sintheta = 1;
		} else {
			costheta = 1.0 / sqrt(1 + m * m);
			sintheta = m * costheta;
		}

		movingright = (dx - curx >= 0) ? true : false;
		movingup = (dy - cury >= 0) ? true : false;

		if (sintheta < 0)
			sintheta = -sintheta;

		if (costheta < 0)
			costheta = -costheta;

		if (movingright) {
			if ((dx == curx)) {
				if (movingup)
					facingAngle = 90;
				else
					facingAngle = 270;
			} else
				facingAngle = (atan(m) * 180.0) / PI;
		} else {
			if ((dx == curx))
				facingAngle = 270;
			else
				facingAngle = 180 + ((atan(m) * 180.0) / PI);
		}

		if (dx == curx && dy == cury) {
			float nm = (perfecty - dy) / (perfectx - dx);
			float tmpang;
			if (perfectx - dx >= 0) {
				tmpang = ((atan(nm) * 180.0) / PI);;
			} else {
				tmpang = 180 + ((atan(nm) * 180.0) / PI);;
			}
			facingAngle = tmpang;
		}
	}

	//checking Collisions
	//with walls
	if (CheckCurWithWalls(dx, dy)) {
		if (movingright) {
			newy = dy + m * (game->map->length / 2 - dx);
			newx = game->map->length / 2 - game->map->blocksize / 2;
		} else {
			newy = dy - m * (dx + game->map->length / 2);
			newx = -game->map->length / 2 + game->map->blocksize / 2;
		}

		if (newy > game->map->breadth / 2) {
			newx = (game->map->breadth / 2 - dy) / m + dx;
			newy = game->map->breadth / 2 - game->map->blocksize / 2;
		} else if (newy < -game->map->breadth / 2) {
			newx = dx - (game->map->breadth / 2 + dy) / m;
			newy = -game->map->breadth / 2 + game->map->blocksize / 2;
		}

		game->block_convert(dx, dy, newx, newy);

	}

	{
		//since dx and dy have changed costheta and sintheta also change
		//we have to change them because we are using them in Run, cal curx and cury

		m = (dy - cury) / (dx - curx);

		if (dx == curx) {
			costheta = 0;
			sintheta = 1;
		} else {
			costheta = 1.0 / sqrt(1 + m * m);
			sintheta = m * costheta;
		}

		movingright = (dx - curx >= 0) ? true : false;
		movingup = (dy - cury >= 0) ? true : false;

		if (sintheta < 0)
			sintheta = -sintheta;

		if (costheta < 0)
			costheta = -costheta;

	}

	if ((perfectx != curx) && (perfecty != cury)) {
		int corner_no = 0, building_no = 0, buildingcorner_no =
		    0, count = 0;
		float add = game->map->blocksize / 2.0;
		float slope;
		float tempx1, tempx2, tempy1, tempy2;

		slope = (perfecty - cury) / (perfectx - curx);

		bool check1, check2;
		bool one, two, three, four;
		int temp, a, b, c, d;
		float mslope;

		for (int i = 0; i < game->map->no_of_buildings; i++) {
			one = two = three = four = false;
			check1 = check2 = false;
			temp = a = b = c = d = 0;

			corner_no = 0;

			tempx1 = game->map->buildings[i].x1 - add;
			tempx2 = game->map->buildings[i].x2 + add;
			tempy1 = game->map->buildings[i].y1 - add;
			tempy2 = game->map->buildings[i].y2 + add;

			one =
			    ((tempy1 - cury - slope * tempx1 + slope * curx) >=
			     0);
			two =
			    ((tempy1 - cury - slope * tempx2 + slope * curx) >=
			     0);
			three =
			    ((tempy2 - cury - slope * tempx2 + slope * curx) >=
			     0);
			four =
			    ((tempy2 - cury - slope * tempx1 + slope * curx) >=
			     0);

			a = (int)one;
			b = (int)two;
			c = (int)three;
			d = (int)four;
			temp = a + b + c + d;

			if (temp == 1)
				corner_no = a * 1 + b * 2 + c * 3 + d * 4;
			else if (temp == 3)
				corner_no =
				    (1 - a) * 1 + (1 - b) * 2 + (1 - c) * 3 +
				    (1 - d) * 4;
			else
				corner_no = 0;

			mslope = -(1.0 / slope);

			switch (corner_no) {
			case 1:
				check1 =
				    (tempy1 - cury - mslope * tempx1 +
				     mslope * curx >= 0);
				check2 =
				    (tempy1 - perfecty - mslope * tempx1 +
				     mslope * perfectx >= 0);
				break;
			case 2:
				check1 =
				    (tempy1 - cury - mslope * tempx2 +
				     mslope * curx >= 0);
				check2 =
				    (tempy1 - perfecty - mslope * tempx2 +
				     mslope * perfectx >= 0);
				break;
			case 3:
				check1 =
				    (tempy2 - cury - mslope * tempx2 +
				     mslope * curx >= 0);
				check2 =
				    (tempy2 - perfecty - mslope * tempx2 +
				     mslope * perfectx >= 0);
				break;
			case 4:
				check1 =
				    (tempy2 - cury - mslope * tempx1 +
				     mslope * curx >= 0);
				check2 =
				    (tempy2 - perfecty - mslope * tempx1 +
				     mslope * perfectx >= 0);
				break;
			default:
				check1 = false;
				check2 = false;
				break;
			}
			if ((check1 ^ check2)) {
				count += 1;
				building_no = i;
				buildingcorner_no = corner_no;
			}
		}

		if (count == 1) {

			destx1 = dx;
			desty1 = dy;

			switch (buildingcorner_no) {
			case 1:
				dx = game->map->buildings[building_no].x1 -
				    game->map->blocksize;
				dy = game->map->buildings[building_no].y1 -
				    game->map->blocksize;
				break;
			case 2:
				dx = game->map->buildings[building_no].x2 +
				    game->map->blocksize;
				dy = game->map->buildings[building_no].y1 -
				    game->map->blocksize;
				break;
			case 3:
				dx = game->map->buildings[building_no].x2 +
				    game->map->blocksize;
				dy = game->map->buildings[building_no].y2 +
				    game->map->blocksize;
				break;
			case 4:
				dx = game->map->buildings[building_no].x1 -
				    game->map->blocksize;
				dy = game->map->buildings[building_no].y2 +
				    game->map->blocksize;
				break;
			default:
				printf
				    ("ERROR ERROR IN INTERMEDIATE CALCULATION\n");
				destx1 = destx = dx;
				desty1 = desty = dy;
				break;
			}

			{
				//since dx and dy have changed costheta and sintheta also change
				//we have to change them because we are using them in Run, cal curx and cury

				m = (dy - cury) / (dx - curx);

				if (dx == curx) {
					costheta = 0;
					sintheta = 1;
				} else {
					costheta = 1.0 / sqrt(1 + m * m);
					sintheta = m * costheta;
				}

				movingright = (dx - curx >= 0) ? true : false;
				movingup = (dy - cury >= 0) ? true : false;

				if (sintheta < 0)
					sintheta = -sintheta;

				if (costheta < 0)
					costheta = -costheta;

			}

			destx = dx;
			desty = dy;

		} else {
			destx1 = destx = dx;
			desty1 = desty = dy;
		}
	}

	return -1;
}

bool Hero::mouseOverMe(float x, float y)
{
	float distance;
	float tempx = curx - x, tempy = cury - y;

	distance = tempx * tempx + tempy * tempy;

	if (distance < 400)
		return true;
	else
		return false;
}

float* Hero::GetBB()
{
    int s = 30;
    bbox[0] = x-s;
    bbox[1] = y-s;
    bbox[2] = z-s;

    bbox[3] = x+s;
    bbox[4] = y+s;
    bbox[5] = z+s;

    return bbox;
}

void Hero::Fov(void)
{
	float ptx1, pty1;
	float tptx1, tpty1;

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fovID);
    glRotatef(180, 0, 1, 0);
	glBegin(GL_POLYGON);

	glTexCoord2f(0.5, 0.5);
	glVertex3f(0, 0, 0);

	for (float i = fovStart; i >= fovEnd; i -= fovInterval) {
		tptx1 = sin(i);
		tpty1 = cos(i);
		ptx1 = fov * tptx1;
		pty1 = fov * tpty1;
		glTexCoord2f(0.5 + 0.5 * tptx1, 0.5 + 0.5 * tpty1);
		glVertex3f(-ptx1, 0, -pty1);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);

}

void Hero::RenderBBox()
{  
    float *bbox = GetBB();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(bbox[0], bbox[1], bbox[2]);
    glVertex3f(bbox[0], bbox[1], bbox[5]);

    glVertex3f(bbox[0], bbox[4], bbox[2]);
    glVertex3f(bbox[0], bbox[4], bbox[5]);

    glVertex3f(bbox[3], bbox[4], bbox[2]);
    glVertex3f(bbox[3], bbox[4], bbox[5]);

    glVertex3f(bbox[3], bbox[1], bbox[2]);
    glVertex3f(bbox[3], bbox[1], bbox[5]);

    glVertex3f(bbox[0], bbox[1], bbox[2]);
    glVertex3f(bbox[0], bbox[1], bbox[5]);

    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

bool Hero::CalculateDest(float x1, float y1, float &x2, float &y2)
{
	return true;
}

void Hero::Render(void)
{
	if (selected) {
		float w = 20;
		float w2 = w / 1.414;
		float h = 3;
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
		glVertex3f(curx + w, h, cury);
		glVertex3f(curx + w2, h, cury + w2);
		glVertex3f(curx, h, cury + w);
		glVertex3f(curx - w2, h, cury + w2);
		glVertex3f(curx - w, h, cury);
		glVertex3f(curx - w2, h, cury - w2);
		glVertex3f(curx, h, cury - w);
		glVertex3f(curx + w2, h, cury - w2);
		glEnd();
	}

	glColor4f(0, 1.0, 0, 1);
	glPushMatrix();
	glTranslatef(curx, 0.3, cury);
	glRotatef(-facingAngle, 0, 1, 0);
	Fov();
	glPopMatrix();
	glColor3f(1, 1, 1);
	basemodel->render(x, y, z, facingAngle);

	if (AttackFrameCount > 0)
		AttackFrameCount--;

	if (DeathFrameCount > 0)
		DeathFrameCount -= 1;
}

void Hero::Dump(void)
{
	printf("HERO: currentpos (%f , %f)\n", curx, cury);
}
