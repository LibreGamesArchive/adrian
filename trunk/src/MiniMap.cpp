#include "MiniMap.h"
#include "globals.h"

MiniMap::MiniMap()
{
	xconvfactor = map->length / 100;
	yconvfactor = map->breadth / 100;

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
	glBindTexture(GL_TEXTURE_2D, map->textureID);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 0);
	glVertex2f((map->length / 2.0) / xconvfactor,
		   (map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(1, 0);
	glVertex2f(-(map->length / 2.0) / xconvfactor,
		   (map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(1, 1);
	glVertex2f(-(map->length / 2.0) / xconvfactor,
		   -(map->breadth / 2.0) / yconvfactor);
	glTexCoord2f(0, 1);
	glVertex2f((map->length / 2.0) / xconvfactor,
		   -(map->breadth / 2.0) / yconvfactor);
	glEnd();

	glColor3f(1, 1, 1);
	for (int i = 0; i < map->no_of_buildings; i++) {
//              glBindTexture( GL_TEXTURE_2D , map->buildings[i].textureID );
		glBegin(GL_POLYGON);
//                      glTexCoord2f( .25, 0);
		glVertex2f(map->buildings[i].x1 / xconvfactor,
			   -map->buildings[i].y1 / yconvfactor);
//                      glTexCoord2f( .25, .75 );
		glVertex2f(map->buildings[i].x1 / xconvfactor,
			   -map->buildings[i].y2 / yconvfactor);
//                      glTexCoord2f( .70, .75);
		glVertex2f(map->buildings[i].x2 / xconvfactor,
			   -map->buildings[i].y2 / yconvfactor);
//                      glTexCoord2f( .70, 0);
		glVertex2f(map->buildings[i].x2 / xconvfactor,
			   -map->buildings[i].y1 / yconvfactor);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glEndList();
	return 0;
}

void MiniMap::Render(void)
{
	glPushMatrix();

	glTranslatef(565, 75, 0);
	glRotatef(camera->angle * 180.0 / 3.141, 0, 0, -1);

	glCallList(mapbuildings);

	glPointSize(3);
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < num_guards; i++) {
		if (guard[i]->Alive)
			glVertex2f(guard[i]->curx / xconvfactor,
				   -guard[i]->cury / yconvfactor);
	}
	glColor3f(1, 1, 1);
	glVertex2f(hero->curx / xconvfactor, -hero->cury / yconvfactor);
	glEnd();

	glPushMatrix();

	glTranslatef(camera->initx / xconvfactor,
		     -((camera->initz)) / yconvfactor, 0);
	glRotatef(camera->angle * 180.0 / 3.141, 0, 0, 1);
	float x, y;
	camera->ConvertCoordinates(320, 240, x, y);
	glColor3f(1, 1, 1);
	float hm = 21.33;
	glLineWidth(1);
//	float my = -((camera->initz)) / yconvfactor;	///yconvfactor;
	glBegin(GL_LINE_STRIP);
	glVertex2f(-hm, -hm);
	glVertex2f(-hm, +hm);
	glVertex2f(+hm, +hm);
	glVertex2f(+hm, -hm);
	glVertex2f(-hm, -hm);
	glEnd();

	glPopMatrix();

	glPopMatrix();

}

bool MiniMap::isMouseOver(int sx, int sy, float &nx, float &ny)
{
	float mx, my;
	mx = (sx - 565) * cos(camera->angle) + (sy - 405) * sin(camera->angle);
	my = (sy - 405) * cos(camera->angle) - (sx - 565) * sin(camera->angle);

	if (mx < 50 && mx > -50 && my < 50 && my > -50) {
		nx = mx;
		ny = my;
		return true;
	}

	return false;
}
