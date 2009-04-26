#include "Map.h"
#include "globals.h"
#include "Game.h"

Map::Map(void)
{
	textureID = 1;
	blocksize = 40;
	textureIDs = NULL;
}

Map::~Map()
{
}

void Map::Initialize(void)
{
}

void Map::Destroy(void)
{
	/* Remove created elements (Guards deleted in Game) */
	delete buildings;

	glDeleteTextures(num_textures, textureIDs);
	free(textureIDs);
}

int Map::LoadFile(const char *filename)
{
	FILE *f;
	char buf[256];

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
		game->block_convert(tempx, tempy, buildings[i].x1, buildings[i].y1);
		buildings[i].x1 = tempx;
		buildings[i].y1 = tempy;
		game->block_convert(tempx, tempy, buildings[i].x2, buildings[i].y2);
		buildings[i].x2 = tempx;
		buildings[i].y2 = tempy;

		buildings[i].bx1 = (int)(length / 2.0 + buildings[i].x1) / blocksize + 1;
		buildings[i].bx2 = (int)(length / 2.0 + buildings[i].x2) / blocksize + 1;
		buildings[i].by1 = (int)(breadth / 2.0 + buildings[i].y1) / blocksize + 1;
		buildings[i].by2 = (int)(breadth / 2.0 + buildings[i].y2) / blocksize + 1;

		buildings[i].setType(buildings[i].buildingType);
	}

	int num_guards;
	fscanf(f, "%d", &num_guards);
	for (int i = 0; i < num_guards; i++) {
		Guard *guard;
		float x[6];
		int texid;
		float botangle;
		fscanf(f, "%s %d %f %f %f %f %f %f", buf, &texid, &x[0], &x[1],
		       &x[2], &x[3], &x[4], &botangle);
		if ((guard = new Guard(buf, texid, x[0], x[1], x[2], x[3], x[4],
								botangle, i)) == NULL) {
			printf("Out of RAM!\n");
			SDL_Quit();
			exit(-1);
		}

		game->addGuard(guard);
	}

	int num_textures;
	fscanf(f, "%d", &num_textures);
	textureIDs = new GLuint[num_textures];
	for (int i = 0; i < num_textures; i++) {
		GLuint texid;
		fscanf(f, "%s %d", buf, &texid);

		int err = loadTGA(buf, texid);
		if (err != 0) {
			printf("Unable to load Texture(%s) into %d: %d\n", buf, texid, err);
			exit(-1);
		}
		printf("Loaded %s into %d\n", buf, texid);

		textureIDs[i] = texid;
	}


	fclose(f);

	return 0;
}

void Map::Render(void)
{
	glEnable(GL_TEXTURE_2D);

	int tilex, tiley;

	const int TILE_SIZE = 300;
	const int TILE_LOWFACTOR = 4.0;
	const int TILE_HIGHFACTOR = 4.0;
	int tileSzFactor = TILE_HIGHFACTOR + TILE_LOWFACTOR;

	/* Render only part of the map (in tiles) which the user can see */
	tilex = ((int)game->camera->initx / TILE_SIZE) * TILE_SIZE;
	tiley = ((int)game->camera->initz / TILE_SIZE) * TILE_SIZE;

	glColor3f(.8, .8, .8);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_POLYGON);
	glTexCoord2f(tileSzFactor, tileSzFactor);
	glVertex3f((tilex - (TILE_LOWFACTOR*TILE_SIZE)), 0, (tiley - (TILE_LOWFACTOR*TILE_SIZE)));
	glTexCoord2f(tileSzFactor, 0);
	glVertex3f((tilex - (TILE_LOWFACTOR*TILE_SIZE)), 0, (tiley + (TILE_HIGHFACTOR*TILE_SIZE)));
	glTexCoord2f(0, 0);
	glVertex3f((tilex + (TILE_HIGHFACTOR*TILE_SIZE)), 0, (tiley + (TILE_HIGHFACTOR*TILE_SIZE)));
	glTexCoord2f(0, tileSzFactor);
	glVertex3f((tilex + (TILE_HIGHFACTOR*TILE_SIZE)), 0, (tiley - (TILE_LOWFACTOR*TILE_SIZE)));
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (game->display_lines) {
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

