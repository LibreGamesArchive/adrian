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


#include "Sprite.h"
#include "Map.h"
#include "globals.h"
#include "Game.h"

bool Sprite::is_bilboard = true;
Sprite::Sprite(GLint texid, float x, float z)
{
	this->texid = texid;
	this->centx = x;
	this->centz = z;
}

Sprite::~Sprite()
{
}

void Sprite::Render(void)
{
	float imgwdth = 60;
	float ang = (Sprite::is_bilboard? game->camera->angle : START_ANGLE);
	/* actually 90 + angle */
	float lenx = cos(ang) * imgwdth;
	float lenz = -sin(ang) * imgwdth;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);

	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texid);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(centx - lenx, 0, centz - lenz);
		glTexCoord2f(1.0, 0);
		glVertex3f(centx - lenx, 150, centz - lenz);
		glTexCoord2f(0, 0);
		glVertex3f(centx + lenx, 150, centz + lenz);
		glTexCoord2f(0, 1.0);
		glVertex3f(centx + lenx, 0, centz + lenz);
	glEnd();
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

