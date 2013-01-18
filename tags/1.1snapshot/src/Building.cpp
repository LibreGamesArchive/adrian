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

#include "Building.h"
#include "globals.h"
#include "Game.h"

Building::Building(void)
{
}

Building::~Building()
{
}

int Building::Load(const char *fn)
{
	FILE *f;
	char buf[1024];
	strncpy(filepath, fn, 256);
	
	if ((f = fopen(filepath, "r")) == NULL) {
		fprintf(stderr, "Cannot open building file: %s\n", filepath);
		exit(-1);
	}

	dlist = glGenLists(1);
	glNewList(dlist, GL_COMPILE);
	while (fgets(buf, 1024, f) != NULL) {
		char *str = buf + 5;
		float fl[5];
		int ret;

		if (!strncmp(buf, "#", 1))
			continue;

		/* Strip trailing '\n' */
		for (int i = 0; str[i]; i++)
			if (str[i] == '\n' || str[i] == '\r')
				str[i] = '\0';

		ret = sscanf(str, "%f,%f,%f,%f,%f", &fl[0], &fl[1], &fl[2], &fl[3], &fl[4]);
		if (!strncmp(buf, "begn:", 5)) {
			GLenum e;
			sscanf(str, "%d", &e);
			glBegin(e);
		} else if (!strncmp(buf, "enbl:", 5)) {
			GLenum e;
			sscanf(str, "%d", &e);
			glEnable(e);
		} else if (!strncmp(buf, "dsbl:", 5)) {
			GLenum e;
			sscanf(str, "%d", &e);
			glDisable(e);
		} else if (!strncmp(buf, "ends", 4)) {
			glEnd();
		} else if (!strncmp(buf, "txco:", 5)) {
			glTexCoord2f(fl[0], fl[1]);
		} else if (!strncmp(buf, "colr:", 5)) {
			glColor3f(fl[0], fl[1], fl[2]);
		} else if (!strncmp(buf, "vrtx:", 5)) {
			glVertex3f(fl[0], fl[1], fl[2]);
		} else if (!strncmp(buf, "txtr:", 5)) {
			GLuint texid = game->map->getTextureID(str);
			printf("TexNAME: %s TEXID:%d\n", str, texid);
			glBindTexture(GL_TEXTURE_2D, texid);
		} else if (!strncmp(buf, "crds:", 5)) {
			h = fl[4];
			printf("Setting h = %f\n", h);
		} else if (!strncmp(buf, "blid:", 5)) {
			int type;
			sscanf(str, "%d", &type);
			buildingID = type;
		} else {
			printf("Ignoring line %s\n", buf);
		}
	}
	glEndList();

	fclose(f);
	return 0;
}

float coff = 10;
bool Building::isInside(float x, float y)
{
	return (x > x1 - coff && x < x2 + coff && y > y1 - coff
		&& y < y2 + coff);
}

void Building::Render(void)
{
	glCallList(dlist);
}

void Building::Dump(void)
{
	printf("Building No : %d (texutureID %d) -- (%f,%f) to (%f,%f)\n",
	       buildingID, textureID, x1, y1, x2, y2);
}
