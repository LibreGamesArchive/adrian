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


#include "MiniMap.h"
#include "globals.h"
#include "Game.h"

MiniMap::MiniMap()
{
	xconvfactor = game->map->length / SCR2RESX(100);
	yconvfactor = game->map->breadth / SCR2RESY(100);

	GenerateDisplayLists();
}

MiniMap::~MiniMap()
{

}

int MiniMap::GenerateDisplayLists(void)
{
	mapbuildings = glGenLists(1);
	glNewList(mapbuildings, GL_COMPILE);
	glColor3f(.8, .8, .8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, game->map->textureID);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1);
	glVertex2f((game->map->length / 2.0) / xconvfactor,
		   (game->map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(1, 1);
	glVertex2f(-(game->map->length / 2.0) / xconvfactor,
		   (game->map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(1, 0);
	glVertex2f(-(game->map->length / 2.0) / xconvfactor,
		   -(game->map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(0, 0);
	glVertex2f((game->map->length / 2.0) / xconvfactor,
		   -(game->map->breadth / 2.0) / yconvfactor);
	glEnd();

	glColor3f(1, 1, 1);
	for (int i = 0; i < game->map->no_of_buildings; i++) {
		glBegin(GL_POLYGON);
		glVertex2f(game->map->buildings[i].x1 / xconvfactor,
			   -game->map->buildings[i].y1 / yconvfactor);
		glVertex2f(game->map->buildings[i].x1 / xconvfactor,
			   -game->map->buildings[i].y2 / yconvfactor);
		glVertex2f(game->map->buildings[i].x2 / xconvfactor,
			   -game->map->buildings[i].y2 / yconvfactor);
		glVertex2f(game->map->buildings[i].x2 / xconvfactor,
			   -game->map->buildings[i].y1 / yconvfactor);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glEndList();
	return 0;
}

void MiniMap::Render(void)
{
	glPushMatrix();

	glTranslatef(SCR2RESX(565), SCR2RESY(75), 0);
	glRotatef(game->camera->angle * 180.0 / 3.141, 0, 0, -1);

	glCallList(mapbuildings);
    GenerateDisplayLists();
	glPointSize(3);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < game->num_guards; i++) {
		if (game->guard[i]->Alive)
			glVertex2f(game->guard[i]->curx / xconvfactor,
				   -game->guard[i]->cury / yconvfactor);
	}
	glColor3f(1, 1, 1);
	glVertex2f(game->hero->curx / xconvfactor, -game->hero->cury / yconvfactor);
	glEnd();

	glPushMatrix();

	glTranslatef(game->camera->initx / xconvfactor,
		     -((game->camera->initz)) / yconvfactor, 0);
	glRotatef(game->camera->angle * 180.0 / 3.141, 0, 0, 1);
	float x, y;
	game->camera->ConvertCoordinates((hres/2.0), (vres/2.0), x, y);
	glColor3f(1, 1, 1);
	float hm = SCR2RESX(21.33);
	float vm = SCR2RESY(21.33);
//	float hm = x, vm = y;
	glLineWidth(1);
//	float my = -((game->camera->initz)) / yconvfactor;	///yconvfactor;
	glBegin(GL_LINE_STRIP);
	glVertex2f(-hm, -vm);
	glVertex2f(-hm, +vm);
	glVertex2f(+hm, +vm);
	glVertex2f(+hm, -vm);
	glVertex2f(-hm, -vm);
	glEnd();

	glPopMatrix();

	glPopMatrix();

}

bool MiniMap::isMouseOver(int sx, int sy, float &nx, float &ny)
{
	float mx, my;
	mx = (sx - SCR2RESX(565)) * cos(game->camera->angle) + (sy - SCR2RESY(405)) * sin(game->camera->angle);
	my = (sy - SCR2RESY(405)) * cos(game->camera->angle) - (sx - SCR2RESX(565)) * sin(game->camera->angle);

	if (mx < SCR2RESX(50) && mx > -SCR2RESX(50) && my < SCR2RESY(50) && my > -SCR2RESY(50)) {
		nx = mx;
		ny = my;
		return true;
	}

	return false;
}
