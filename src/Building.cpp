#include "Building.h"
#include "globals.h"
#include "tga.h"

Building::Building(void)
{
	//texture coardinates will be specified in a file
	//load them while creating an instance  
	h = 150;
}

Building::~Building()
{
}

float coff = 10;
bool Building::isInside(float x, float y)
{
	return (x > x1 - coff && x < x2 + coff && y > y1 - coff
		&& y < y2 + coff);
}

void Building::setType(int type)
{
	buildingID = type;
	switch (buildingID) {
	case 1:
	case 4:
		h = 150;
		break;
	case 2:
		h = 100;
		break;
	case 3:
		h = 75;
		break;
	}
}

void bunkerpole(float x1, float y1)
{
	float h = 40;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 23);	//ID 8 for missile.tga
	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0.5, 1);
	glVertex3f(x1, h, y1);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1 - 5, 0, y1 + 5);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(x1 + 5, 0, y1);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(x1 - 5, 0, y1 - 5);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(x1 - 5, 0, y1 + 5);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Building::Render(void)
{
	switch (buildingID) {
	case 10:		//WALL VERTICAL
		{
			h = 60;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glColor3f(1, 1, 1);

			float xr = (x2 - x1) / 100.0, yr = (y2 - y1) / 100.0;

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glTexCoord2f(xr, 1);
			glVertex3f(x2, h, y1);
			glTexCoord2f(xr, 0);
			glVertex3f(x2, 0, y1);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y2);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y2);
			glTexCoord2f(xr, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(xr, 0);
			glVertex3f(x2, 0, y2);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glTexCoord2f(yr, 1);
			glVertex3f(x1, h, y2);
			glTexCoord2f(yr, 0);
			glVertex3f(x1, 0, y2);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x2, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x2, h, y1);
			glTexCoord2f(yr, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(yr, 0);
			glVertex3f(x2, 0, y2);
			glEnd();

			glColor3f(0, 0, 0);
			//TOP
			glBegin(GL_POLYGON);
			glVertex3f(x1, h, y1);
			glVertex3f(x1, h, y2);
			glVertex3f(x2, h, y2);
			glVertex3f(x2, h, y1);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			break;
		}
	case 9:		//GB general's building
		{
			h = 100;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glColor3f(1, 1, 1);

			//DOOR
			glBegin(GL_POLYGON);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2, 0, y1);
			glTexCoord2f(0.5, 1.0);
			glVertex3f(x2, h, y1);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(x2, h, y2);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x2, 0, y2);
			glEnd();

			//BACK
			glBegin(GL_POLYGON);
			glTexCoord2f(0.5, 0.0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x1, h, y1);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x1, h, y2);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(x1, 0, y2);
			glEnd();

			//SIDE1
			glBegin(GL_POLYGON);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(1.0, 0.5);
			glVertex3f(x1, h, y1);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2, h, y1);
			glTexCoord2f(0.5, 0.0);
			glVertex3f(x2, 0, y1);
			glEnd();

			//SIDE2
			glBegin(GL_POLYGON);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x1, 0, y2);
			glTexCoord2f(1.0, 0.5);
			glVertex3f(x1, h, y2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(x2, h, y2);
			glTexCoord2f(0.5, 1.0);
			glVertex3f(x2, 0, y2);
			glEnd();

			//TOP
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x1, h, y1);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2 + 120, h, y1);
			glTexCoord2f(0.5, 0.45);
			glVertex3f(x2 + 120, h, y2);
			glTexCoord2f(0.0, 0.45);
			glVertex3f(x1, h, y2);
			glEnd();
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x2, h, y1);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2, h - 10, y1);
			glTexCoord2f(0.5, 0.45);
			glVertex3f(x2 + 120, h - 10, y1);
			glTexCoord2f(0.0, 0.45);
			glVertex3f(x2 + 120, h, y1);
			glEnd();
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x2, h, y2);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2, h - 10, y2);
			glTexCoord2f(0.5, 0.45);
			glVertex3f(x2 + 120, h - 10, y2);
			glTexCoord2f(0.0, 0.45);
			glVertex3f(x2 + 120, h, y2);
			glEnd();
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(x2 + 120, h, y2);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(x2 + 120, h - 10, y2);
			glTexCoord2f(0.5, 0.45);
			glVertex3f(x2 + 120, h - 10, y1);
			glTexCoord2f(0.0, 0.45);
			glVertex3f(x2 + 120, h, y1);
			glEnd();

			glDisable(GL_TEXTURE_2D);

			break;
		}
	case 11:		//MANY CRATE crate this should be four grid size
		{
			h = 40;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glColor3f(1, 1, 1);

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glTexCoord2f(1, 0);
			glVertex3f(x2, 0, y1);
			glTexCoord2f(1, 1);
			glVertex3f(x2, h, y1);
			glTexCoord2f(0, 0);
			glVertex3f(x2, 0, y2);
			glTexCoord2f(0, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(1, 0);
			glVertex3f(x1, 0, y2);
			glTexCoord2f(1, 1);
			glVertex3f(x1, h, y2);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x1, h, y1);
			glTexCoord2f(1, 0);
			glVertex3f(x2, h, y1);
			glTexCoord2f(1, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y2);
			glEnd();

//                      h1 = 20;
//                      h2 = 40; 
			/*      
			   glBegin( GL_QUAD_STRIP );
			   glTexCoord2f(0,0);
			   glVertex3f(x1,0,y1);
			   glTexCoord2f(0,1);
			   glVertex3f(x1,h,y1);
			   glTexCoord2f(1,0);
			   glVertex3f(x2,0,y1);
			   glTexCoord2f(1,1);
			   glVertex3f(x2,h,y1);
			   glTexCoord2f(0,0);
			   glVertex3f(x2,0,y2);
			   glTexCoord2f(0,1);
			   glVertex3f(x2,h,y2);
			   glTexCoord2f(1,0);
			   glVertex3f(x1,0,y2);
			   glTexCoord2f(1,1);
			   glVertex3f(x1,h,y2);
			   glTexCoord2f(0,0);
			   glVertex3f(x1,0,y1);
			   glTexCoord2f(0,1);
			   glVertex3f(x1,h,y1);
			   glEnd( );

			   glBegin(GL_POLYGON);
			   glTexCoord2f(0,0);
			   glVertex3f(x1,h,y1);
			   glTexCoord2f(1,0);
			   glVertex3f(x2,h,y1);
			   glTexCoord2f(1,1);
			   glVertex3f(x2,h,y2);
			   glTexCoord2f(0,1);
			   glVertex3f(x1,h,y2);
			   glEnd();
			 */
			glDisable(GL_TEXTURE_2D);
			break;
		}
	case 6:		//CRATE crate this should be four grid size
		{
			h = 40;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glColor3f(1, 1, 1);

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glTexCoord2f(1, 0);
			glVertex3f(x2, 0, y1);
			glTexCoord2f(1, 1);
			glVertex3f(x2, h, y1);
			glTexCoord2f(0, 0);
			glVertex3f(x2, 0, y2);
			glTexCoord2f(0, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(1, 0);
			glVertex3f(x1, 0, y2);
			glTexCoord2f(1, 1);
			glVertex3f(x1, h, y2);
			glTexCoord2f(0, 0);
			glVertex3f(x1, 0, y1);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y1);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(x1, h, y1);
			glTexCoord2f(1, 0);
			glVertex3f(x2, h, y1);
			glTexCoord2f(1, 1);
			glVertex3f(x2, h, y2);
			glTexCoord2f(0, 1);
			glVertex3f(x1, h, y2);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			break;
		}
	case 8:		//BUNKER
		{
			float cx = x1 + (x2 - x1) / 2;
			float cy = y1 + (y2 - y1) / 2;
			float r1 = (x2 - x1) / 2;
			float r2 = r1 - 10;
			float tr1 = r1 / 1.414, tr2 = r2 / 1.414;
			float h1, h2;
			h1 = 0;
			h2 = h = 40;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glColor3f(0, 0, 0);
			glBegin(GL_POLYGON);
			glVertex3f(cx, 0.1, cy + r1);
			glVertex3f(cx + tr1, 0.1, cy + tr1);
			glVertex3f(cx + r1, 0.1, cy);
			glVertex3f(cx + tr1, 0.1, cy - tr1);
			glVertex3f(cx, 0.1, cy - r1);
			glVertex3f(cx - tr1, 0.1, cy - tr1);
			glVertex3f(cx - r1, 0.1, cy);
			glVertex3f(cx - tr1, 0.1, cy + tr1);
			glEnd();

			glColor3f(1, 1, 1);
			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy - r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy - r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx - r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glEnd();

			r1 = r2;
			tr1 = tr2;
			h2 = 70;
			h1 = 50;
			h = 70;

			glColor3f(0, 0, 0);
			glBegin(GL_POLYGON);
//                              glTexCoord2f(0.5,1);
			glVertex3f(cx, h, cy + r2);
//                              glTexCoord2f(0.65,0.65);
			glVertex3f(cx + tr2, h, cy + tr2);
//                              glTexCoord2f(1.0,0.5);
			glVertex3f(cx + r2, h, cy);
//                              glTexCoord2f(0.65,0.35);
			glVertex3f(cx + tr2, h, cy - tr2);
//                              glTexCoord2f(0.5,0.0);
			glVertex3f(cx, h, cy - r2);
//                              glTexCoord2f(0.35,0.35);
			glVertex3f(cx - tr2, h, cy - tr2);
//                              glTexCoord2f(0.0,0.5);
			glVertex3f(cx - r2, h, cy);
//                              glTexCoord2f(0.35,0.65);
			glVertex3f(cx - tr2, h, cy + tr2);
			glEnd();

			glColor3f(1, 1, 1);
			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy - r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy - r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx - r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glEnd();

			glDisable(GL_TEXTURE_2D);

			bunkerpole(x1, y1);
			bunkerpole(x1 + map->blocksize, y1);
			bunkerpole(x1, y1 + map->blocksize);
			bunkerpole(x2, y1);
			bunkerpole(x2 - map->blocksize, y1);
			bunkerpole(x2, y1 + map->blocksize);
			bunkerpole(x2, y2);
			bunkerpole(x2 - map->blocksize, y2);
			bunkerpole(x2, y2 - map->blocksize);
			bunkerpole(x1, y2);
			bunkerpole(x1 + map->blocksize, y2);
			bunkerpole(x1, y2 - map->blocksize);

			break;
		}

	case 4:		//ANTI AIR CRAFT GUN
		{
			float cx = x1 + (x2 - x1) / 2;
			float cy = y1 + (y2 - y1) / 2;
			float r1 = (x2 - x1) / 2;
			float r2 = r1 - 10;
			float tr1 = r1 / 1.414, tr2 = r2 / 1.414;
			float h1, h2;
			h1 = 0;
			h2 = h = 20;

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(0.5, 1);
			glVertex3f(cx, h, cy + r2);
			glTexCoord2f(0.65, 0.65);
			glVertex3f(cx + tr2, h, cy + tr2);
			glTexCoord2f(1.0, 0.5);
			glVertex3f(cx + r2, h, cy);
			glTexCoord2f(0.65, 0.35);
			glVertex3f(cx + tr2, h, cy - tr2);
			glTexCoord2f(0.5, 0.0);
			glVertex3f(cx, h, cy - r2);
			glTexCoord2f(0.35, 0.35);
			glVertex3f(cx - tr2, h, cy - tr2);
			glTexCoord2f(0.0, 0.5);
			glVertex3f(cx - r2, h, cy);
			glTexCoord2f(0.35, 0.65);
			glVertex3f(cx - tr2, h, cy + tr2);
			glEnd();

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy - r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy - r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx - r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glEnd();

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + 2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy + 2);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx, h2, cy + 12);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy + 12);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2 + 10, cy + 7);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + 150, h2 + 60, cy + 7);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx, h2, cy + 2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy + 2);
			glEnd();

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx, h2, cy - 10);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy - 10);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2 + 10, cy - 5);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + 150, h2 + 60, cy - 5);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx, h2, cy);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + 150, h2 + 50, cy);
			glEnd();

			r1 = r2;
			tr1 = tr2;
			h2 = 40;
			h1 = 20;

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy - r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy - r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy - tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy - tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx - r2, h2, cy);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h1, cy);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - tr2, h2, cy + tr2);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - tr1, h1, cy + tr1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx, h2, cy + r2);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx, h1, cy + r1);
			glEnd();

			r1 = r1 / 3;
			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx - r1, h1, cy + r1);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h2, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + r1, h1, cy + r1);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx + r1, h2, cy + r1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r1, h1, cy - r1);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx + r1, h2, cy - r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx - r1, h1, cy - r1);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - r1, h2, cy - r1);
			glEnd();

			glBegin(GL_POLYGON);
			glTexCoord2f(1.0, 1.0);
			glVertex3f(cx - r1, h2, cy + r1);
			glTexCoord2f(1.0, 0.0);
			glVertex3f(cx + r1, h2, cy + r1);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(cx + r1, h2, cy - r1);
			glTexCoord2f(0.0, 1.0);
			glVertex3f(cx - r1, h2, cy - r1);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			break;
		}

	case 5:		//TANK
		{
			h = 99;

			float w = (x2 - x1 > y2 - y1) ? x2 - x1 : y2 - y1;
			w /= 2;
			float w2 = w / 1.414;
			float curx = x1 + (x2 - x1) / 2;
			float cury = y1 + (y2 - y1) / 2;
			float texw = .5;
			float texw2 = texw / 1.414;

			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(.5 + texw, .5);
			glVertex3f(curx + w, h, cury);
			glTexCoord2f(.5 + texw2, .5 + texw2);
			glVertex3f(curx + w2, h, cury + w2);
			glTexCoord2f(.5, .5 + texw);
			glVertex3f(curx, h, cury + w);
			glTexCoord2f(.5 - texw2, .5 + texw2);
			glVertex3f(curx - w2, h, cury + w2);
			glTexCoord2f(.5 - texw, .5);
			glVertex3f(curx - w, h, cury);
			glTexCoord2f(.5 - texw2, .5 - texw2);
			glVertex3f(curx - w2, h, cury - w2);
			glTexCoord2f(.5, .5 - texw);
			glVertex3f(curx, h, cury - w);
			glTexCoord2f(.5 + texw2, .5 - texw2);
			glVertex3f(curx + w2, h, cury - w2);
			glEnd();

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0, 0);
			glVertex3f(curx + w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx + w, h, cury);
			glTexCoord2f(1, 0);
			glVertex3f(curx + w2, 0, cury + w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx + w2, h, cury + w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx, 0, cury + w);
			glTexCoord2f(0, 1);
			glVertex3f(curx, h, cury + w);
			glTexCoord2f(1, 0);
			glVertex3f(curx - w2, 0, cury + w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx - w2, h, cury + w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx - w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx - w, h, cury);
			glTexCoord2f(1, 0);
			glVertex3f(curx - w2, 0, cury - w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx - w2, h, cury - w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx, 0, cury - w);
			glTexCoord2f(0, 1);
			glVertex3f(curx, h, cury - w);
			glTexCoord2f(1, 0);
			glVertex3f(curx + w2, 0, cury - w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx + w2, h, cury - w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx + w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx + w, h, cury);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		}
	case 7:		//BARREL        four grid size
		{
			h = 40;
			float w = (x2 - x1 > y2 - y1) ? x2 - x1 : y2 - y1;
			w /= 2;
			float w2 = w / 1.414;
			float curx = x1 + (x2 - x1) / 2;
			float cury = y1 + (y2 - y1) / 2;
			float texw = .5;
			float texw2 = texw / 1.414;

			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glColor3f(1, 1, 1);
			glBegin(GL_POLYGON);
			glTexCoord2f(.5 + texw, .5);
			glVertex3f(curx + w, h, cury);
			glTexCoord2f(.5 + texw2, .5 + texw2);
			glVertex3f(curx + w2, h, cury + w2);
			glTexCoord2f(.5, .5 + texw);
			glVertex3f(curx, h, cury + w);
			glTexCoord2f(.5 - texw2, .5 + texw2);
			glVertex3f(curx - w2, h, cury + w2);
			glTexCoord2f(.5 - texw, .5);
			glVertex3f(curx - w, h, cury);
			glTexCoord2f(.5 - texw2, .5 - texw2);
			glVertex3f(curx - w2, h, cury - w2);
			glTexCoord2f(.5, .5 - texw);
			glVertex3f(curx, h, cury - w);
			glTexCoord2f(.5 + texw2, .5 - texw2);
			glVertex3f(curx + w2, h, cury - w2);
			glEnd();

			glBegin(GL_QUAD_STRIP);
			glTexCoord2f(0, 0);
			glVertex3f(curx + w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx + w, h, cury);
			glTexCoord2f(1, 0);
			glVertex3f(curx + w2, 0, cury + w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx + w2, h, cury + w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx, 0, cury + w);
			glTexCoord2f(0, 1);
			glVertex3f(curx, h, cury + w);
			glTexCoord2f(1, 0);
			glVertex3f(curx - w2, 0, cury + w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx - w2, h, cury + w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx - w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx - w, h, cury);
			glTexCoord2f(1, 0);
			glVertex3f(curx - w2, 0, cury - w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx - w2, h, cury - w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx, 0, cury - w);
			glTexCoord2f(0, 1);
			glVertex3f(curx, h, cury - w);
			glTexCoord2f(1, 0);
			glVertex3f(curx + w2, 0, cury - w2);
			glTexCoord2f(1, 1);
			glVertex3f(curx + w2, h, cury - w2);
			glTexCoord2f(0, 0);
			glVertex3f(curx + w, 0, cury);
			glTexCoord2f(0, 1);
			glVertex3f(curx + w, h, cury);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			break;
		}
	case 1:
		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//clockwise, left top vertex first
		//WALLS
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glTexCoord2f(1, 0);
		glVertex3f(x1, 0, y2);
		glTexCoord2f(1, 1);
		glVertex3f(x1, h, y2);

		glTexCoord2f(0, 0);
		glVertex3f(x2, 0, y2);
		glTexCoord2f(0, 1);
		glVertex3f(x2, h, y2);
		glTexCoord2f(1, 0);
		glVertex3f(x2, 0, y1);
		glTexCoord2f(1, 1);
		glVertex3f(x2, h, y1);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glEnd();

		//TOP
		glBegin(GL_POLYGON);
		glTexCoord2f(.25, 0);
		glVertex3f(x1, h, y1);
		glTexCoord2f(.25, .75);
		glVertex3f(x1, h, y2);
		glTexCoord2f(.70, .75);
		glVertex3f(x2, h, y2);
		glTexCoord2f(.70, 0);
		glVertex3f(x2, h, y1);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		break;

	case 2:

		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//clockwise, left top vertex first
		//WALLS
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glTexCoord2f(1, 0);
		glVertex3f(x1, 0, y2);
		glTexCoord2f(1, 1);
		glVertex3f(x1, h, y2);

		glTexCoord2f(0, 0);
		glVertex3f(x2, 0, y2);
		glTexCoord2f(0, 1);
		glVertex3f(x2, h, y2);
		glTexCoord2f(1, 0);
		glVertex3f(x2, 0, y1);
		glTexCoord2f(1, 1);
		glVertex3f(x2, h, y1);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glEnd();

		//TOP
		glBegin(GL_POLYGON);
		glTexCoord2f(.25, 0);
		glVertex3f(x1, h, y1);
		glTexCoord2f(.25, .75);
		glVertex3f(x1, h, y2);
		glTexCoord2f(.70, .75);
		glVertex3f(x2, h, y2);
		glTexCoord2f(.70, 0);
		glVertex3f(x2, h, y1);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		break;

	case 3:

		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//clockwise, left top vertex first
		//WALLS
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glTexCoord2f(1, 0);
		glVertex3f(x1, 0, y2);
		glTexCoord2f(1, 1);
		glVertex3f(x1, h, y2);

		glTexCoord2f(0, 0);
		glVertex3f(x2, 0, y2);
		glTexCoord2f(0, 1);
		glVertex3f(x2, h, y2);
		glTexCoord2f(1, 0);
		glVertex3f(x2, 0, y1);
		glTexCoord2f(1, 1);
		glVertex3f(x2, h, y1);
		glTexCoord2f(0, 0);
		glVertex3f(x1, 0, y1);
		glTexCoord2f(0, 1);
		glVertex3f(x1, h, y1);
		glEnd();

		//TOP
		glBegin(GL_POLYGON);
		glTexCoord2f(.25, 0);
		glVertex3f(x1, h, y1);
		glTexCoord2f(.25, .75);
		glVertex3f(x1, h, y2);
		glTexCoord2f(.70, .75);
		glVertex3f(x2, h, y2);
		glTexCoord2f(.70, 0);
		glVertex3f(x2, h, y1);
		glEnd();
		glDisable(GL_TEXTURE_2D);

		break;

	}

}

void Building::Dump(void)
{
	printf("Building No : %d (texutureID %d) -- (%f,%f) to (%f,%f)\n",
	       buildingID, textureID, x1, y1, x2, y2);
}
