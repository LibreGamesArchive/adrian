#include "Map.h"
#include "globals.h"

extern bool display_lines;

Map::Map(void)
{
	textureID = 1;
	blocksize = 40;
}

Map::~Map()
{
}

void Map::Initialize(void)
{
	for (int i = 0; i < num_guards; i++)
		guard[i]->Initialize();
}

int Map::LoadFile(const char *filename)
{
	FILE *f;

	if ((f = fopen(filename, "r")) < 0) {
		printf("Error Opening File : %s\n", filename);
		return -1;
	}

	if (fscanf(f, "%f %f", &length, &breadth) <= 1) {
		printf("Error Reading Length, Breath\n");
		return -2;
	}

	no_blocksx = (int)length / blocksize;
	no_blocksy = (int)breadth / blocksize;

	fscanf(f, "%d", &no_of_buildings);
	buildings = new Building[no_of_buildings];
	for (int i = 0; i < no_of_buildings; i++) {
		buildings[i].buildingID = i;
		fscanf(f, "%f %f %f %f %d %d", &buildings[i].x1,
		       &buildings[i].y1, &buildings[i].x2, &buildings[i].y2,
		       &buildings[i].buildingType, &buildings[i].textureID);
		//      fprintf(stdout,"%f %f %f %f %d %d\n",buildings[i].x1,buildings[i].y1,buildings[i].x2,buildings[i].y2, buildings[i].buildingType, buildings[i].textureID );

		float tempx, tempy;
		block_convert(tempx, tempy, buildings[i].x1, buildings[i].y1);
		buildings[i].x1 = tempx;
		buildings[i].y1 = tempy;
		block_convert(tempx, tempy, buildings[i].x2, buildings[i].y2);
		buildings[i].x2 = tempx;
		buildings[i].y2 = tempy;

		buildings[i].bx1 =
		    (int)(map->length / 2.0 +
			  map->buildings[i].x1) / map->blocksize + 1;
		buildings[i].bx2 =
		    (int)(map->length / 2.0 +
			  map->buildings[i].x2) / map->blocksize + 1;
		buildings[i].by1 =
		    (int)(map->breadth / 2.0 +
			  map->buildings[i].y1) / map->blocksize + 1;
		buildings[i].by2 =
		    (int)(map->breadth / 2.0 +
			  map->buildings[i].y2) / map->blocksize + 1;

		buildings[i].setType(buildings[i].buildingType);
	}

	fscanf(f, "%d", &num_guards);
	for (int i = 0; i < num_guards; i++) {
		char buf[256];
		float x[6];
		int texid;
		float botangle;
		fscanf(f, "%s %d %f %f %f %f %f %f", buf, &texid, &x[0], &x[1],
		       &x[2], &x[3], &x[4], &botangle);
		if ((guard[i] =
		     new Guard(buf, texid, x[0], x[1], x[2], x[3], x[4],
			       botangle, i)) == NULL) {
			printf("Out of RAM!\n");
			SDL_Quit();
			exit(-1);
		}
	}

	fclose(f);

	return 0;
}

void Map::Render(void)
{
	glEnable(GL_TEXTURE_2D);
//      glColor3f(.5,.5,.5);
	glColor3f(.8, .8, .8);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_POLYGON);
	glTexCoord2f(length / 256, breadth / 256);
	glVertex3f(-length, 0, -breadth);
	glTexCoord2f(length / 256, 0);
	glVertex3f(-length, 0, breadth);
	glTexCoord2f(0, 0);
	glVertex3f(length, 0, breadth);
	glTexCoord2f(0, breadth / 256);
	glVertex3f(length, 0, -breadth);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (display_lines) {
		glColor3f(1, 0, 0);
		for (int i = 0; i < no_blocksx; i++) {
			glBegin(GL_LINES);
			glVertex3f(-length / 2.0 + i * blocksize, 0.2,
				   breadth / 2);
			glVertex3f(-length / 2.0 + i * blocksize, 0.2,
				   -breadth / 2);
			glEnd();
			glBegin(GL_LINES);
			glVertex3f(length / 2.0, 0.3,
				   -breadth / 2 + i * blocksize);
			glVertex3f(-length / 2.0, 0.3,
				   -breadth / 2 + i * blocksize);
			glEnd();
		}
	}

//    float farthestdist = 2 * ((hres/2.0) *(hres/2.0) + (vres/2.0) *(vres/2.0));
//      printf("Rendering Polygons : ");
	for (int i = 0; i < no_of_buildings; i++) {
//              if( (buildings[i].x1 - camera->initx)*(buildings[i].x2 - camera->initx) +
//                      (buildings[i].y1 - camera->initz)*(buildings[i].y2 - camera->initz) <= farthestdist )
//              {
//              printf("%d ",i);
		buildings[i].Render();
//              }
	}
//      printf(" : ONLY out of %d buildings\n",no_of_buildings);

}

void Map::Dump(void)
{
	printf("Map : ( %f , %f )\n", length, breadth);
}
