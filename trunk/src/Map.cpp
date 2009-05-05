#include "Map.h"
#include "globals.h"
#include "Game.h"

Map::Map(void)
{
	textureID = 1;
	blocksize = 40;
	textures = NULL;
	sprites = NULL;
}

Map::~Map()
{
}

void Map::Initialize(void)
{
}

void Map::Destroy(void)
{
	soundSystem->UnloadAll();

	for (int i = 0; i < num_sprites; i++)
		delete sprites[i];
	delete sprites;

	/* Remove created elements (Guards deleted in Game) */
	//delete buildings;
	//TODO??

	for (int i = 0; i < num_textures; i++)
		delete textures[i];
	delete[] textures;
	textures = NULL;
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

	int num_textures;
	fscanf(f, "%d", &num_textures);
	textures = new Texture*[num_textures];
	for (int i = 0; i < num_textures; i++) {
		GLuint texid;
		fscanf(f, "%s %d", buf, &texid);

		Texture *t = new Texture(buf);
		if (t->Load(texid) < 0) {
			printf("Unable to load Texture(%s) into %d\n", buf, texid);
			exit(-1);
		}
		printf("Loaded %s into %d\n", buf, texid);

		textures[i] = t;
	}

	/* Load the guards at the LAST! since otherwise
	 * their textures will be overwritten */
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

	fscanf(f, "%d", &num_sprites);
	sprites = new Sprite*[num_sprites];
	for (int i = 0; i < num_sprites; i++) {
		GLuint texid;
		float x, z;
		fscanf(f, "%d %f %f", &texid, &x, &z);
		printf("Loading sprite with tex=%d x=%f z=%f\n", texid, x, z);

		/* Init the sprite */
		sprites[i] = new Sprite(texid, x, z);

	}

	int num_sounds;
	fscanf(f, "%d", &num_sounds);
	for (int i = 0; i < num_sounds; i++) {
		char buf[256];
		int id, loop;
		fscanf(f, "%s %d %d", buf, &id, &loop);
		printf("Loading Sound File: %s with id=%d loop=%d\n", buf, id, loop);

		soundSystem->Load(buf, id, loop);
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
	glTexCoord2f(tileSzFactor, 0);
	glVertex3f((tilex - (TILE_LOWFACTOR*TILE_SIZE)), 0, (tiley - (TILE_LOWFACTOR*TILE_SIZE)));
	glTexCoord2f(tileSzFactor, tileSzFactor);
	glVertex3f((tilex - (TILE_LOWFACTOR*TILE_SIZE)), 0, (tiley + (TILE_HIGHFACTOR*TILE_SIZE)));
	glTexCoord2f(0, tileSzFactor);
	glVertex3f((tilex + (TILE_HIGHFACTOR*TILE_SIZE)), 0, (tiley + (TILE_HIGHFACTOR*TILE_SIZE)));
	glTexCoord2f(0, 0);
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
}

void Map::TransparentRender(void)
{
	for (int i = 0; i < num_sprites; i++) {
		sprites[i]->Render();
	}
}

void Map::Dump(void)
{
	printf("Map : ( %f , %f )\n", length, breadth);
}

