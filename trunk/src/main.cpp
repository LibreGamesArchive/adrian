#include "main.h"
#include "Md2.h"
#include "Guard.h"
#include "globals.h"
#include "Menu.h"
#include "cursor.h"
#include "Panel.h"
#include "config.h"

#include <stdio.h>
#include<stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <signal.h>
#include <unistd.h>

#define			TIME_INTERVAL			300
extern Uint32 TimerCallback(unsigned int);

extern SDL_Cursor *init_system_cursor(const char *image[]);

int width = hres;
int height = vres;

bool Bool_Cam_RotateP = false;
bool Bool_Cam_RotateN = false;
bool Bool_Cam_Rotate_WheelP = false;
bool Bool_Cam_Rotate_WheelN = false;
bool Bool_Cam_MvyP = false;
bool Bool_Cam_MvyN = false;
bool Bool_Cam_MvxP = false;
bool Bool_Cam_MvxN = false;

bool Bool_Mouse_HotAreaRight = false;
bool Bool_Mouse_HotAreaLeft = false;
bool Bool_Mouse_HotAreaTop = false;
bool Bool_Mouse_HotAreaBottom = false;

bool display_lines = false;
bool show_minimap = true;

float angle = START_ANGLE;

void SIGSEGV_HANDLER(int signum)
{
	printf("Segmentaion Fault\n");
	system("~/reset");
	system("killall -9 a.out");
//      SDL_Quit();
//      exit(0);
}

void Quit(int val)
{
	SDL_SetTimer(TIME_INTERVAL, NULL);
	SDL_Quit();
	exit(val);
}

void LoadTextures(void)
{
	// LOADS ALL TEXTURES

	//Map
	if (!loadTGA("maps/ground.tga", 1))
		printf("Unable To Load Map Texture\n");

	//Menu
	if (!loadTGA("maps/menu.tga", 2))
		printf("Unable To Load Map Texture\n");

	//Line Of Sight
	if (!loadTGA("maps/los.tga", 3))
		printf("Unable To Load Map Texture\n");

	//Help
	if (!loadTGA("maps/help.tga", 100))
		printf("Unable To Load Map Texture\n");

	//Guards
	//next 12 id's for guard bots

	//Buildings

	if (!loadTGA("maps/200x200building.tga", 16))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/generalsbuilding.tga", 17))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/wall.tga", 18))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/bunker.tga", 19))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/aircraftgun.tga", 20))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/barrel.tga", 21))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/crate.tga", 22))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/metal.tga", 23))
		printf("Unable To Load Map Texture\n");

	if (!loadTGA("maps/font.tga", 50))
		printf("ERROR LOADING ALPHA TEXTURE\n");

	if (!loadTGA("maps/wall.tga", 30))
		printf("Ihave to check\n");

	//The Panel Textures
	//Panel Bot Texture For Hero
	if (!loadTGA("maps/panel.tga", 62))
		printf("Error loading Panel Texture\n");

	if (!loadTGA("maps/panelhero.tga", 65))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/MutantManPanel.tga", 66))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/CorpsePanel.tga", 67))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/SkelPanel.tga", 68))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/MutantCheeta.tga", 69))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/MutantLizard.tga", 70))
		printf("Error Loading Panel Texture\n");

	if (!loadTGA("maps/Ripper.tga", 71))
		printf("Error Loading Panel Texture\n");

	fontInit();
}

void setup_opengl_menu(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, hres, 0, vres);

	glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
	glLoadIdentity();	// Reset The Modelview Matrix

	glClearColor(0, 0, 0, 0);
	glDisable(GL_DEPTH_TEST);

}

void setup_opengl_game(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(hres/2.0), (hres/2.0), -(vres/2.0), (vres/2.0), -hres, hres);

	glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
	glLoadIdentity();	// Reset The Modelview Matrix

	//3D
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	/*{        // ENABLING FOG
	   float fogColor[4] = {0.0, 0.0, 0.0, 1.0f};   // Let's make the Fog Color black too

	   glFogi(GL_FOG_MODE, GL_EXP2);                          // Set The Fog Mode
	   glFogfv(GL_FOG_COLOR, fogColor);                     // Set The Fog Color
	   glFogf(GL_FOG_DENSITY, 0.003f);                 // Set How Dense Will The Fog Be
	   glHint(GL_FOG_HINT, GL_DONT_CARE);           // Set The Fog's calculation accuracy
	   //glHint(GL_FOG_HINT, GL_NICEST);             // Set The Fog's calculation accuracy
	   glFogf(GL_FOG_START, -600);                          // Set The Fog's Start Depth
	   glFogf(GL_FOG_END, 600);                     // Set The Fog's End Depth
	   glEnable (GL_FOG);
	   } */

}

void Initialize(void)
{

	Bool_Cam_RotateP = false;
	Bool_Cam_RotateN = false;
	Bool_Cam_Rotate_WheelP = false;
	Bool_Cam_Rotate_WheelN = false;

	Bool_Cam_MvyP = false;
	Bool_Cam_MvyN = false;
	Bool_Cam_MvxP = false;
	Bool_Cam_MvxN = false;

	Bool_Mouse_HotAreaRight = false;
	Bool_Mouse_HotAreaLeft = false;
	Bool_Mouse_HotAreaTop = false;
	Bool_Mouse_HotAreaBottom = false;

	display_lines = false;
	show_minimap = true;
	showHelp = false;
	angle = START_ANGLE;
}

void WorldCamUpdate(void)
{
	if (Bool_Cam_Rotate_WheelP) {
		angle += 0.09;
		camera->Rotate(angle);
		Bool_Cam_Rotate_WheelP = false;
	}
	if (Bool_Cam_Rotate_WheelN) {
		angle -= 0.09;
		camera->Rotate(angle);
		Bool_Cam_Rotate_WheelN = false;
	}
	if (Bool_Cam_RotateP) {
		angle += 0.015;
		camera->Rotate(angle);
	}
	if (Bool_Cam_RotateN) {
		angle -= 0.015;
		camera->Rotate(angle);
	}
	if (Bool_Cam_MvyP || Bool_Mouse_HotAreaTop) {
		camera->MoveUp();
	}
	if (Bool_Cam_MvyN || Bool_Mouse_HotAreaBottom) {
		camera->MoveDown();
	}
	if (Bool_Cam_MvxP || Bool_Mouse_HotAreaRight) {
		camera->MoveLeft();
	}
	if (Bool_Cam_MvxN || Bool_Mouse_HotAreaLeft) {
		camera->MoveRight();
	}
}

void newpickObjects(int x, int y)
{
	float newx, newy;	//newx newy are coordinates of clicked pt on map

	camera->ConvertCoordinates(x, y, newx, newy);

	if (hero->mouseOverMe(newx, newy)) {
		hero->selected = true;
		PanelBotTexId = hero->PanelTexId;
	} else {
		hero->selected = false;
	}

	for (int i = 0; i < num_guards; i++) {
		if (guard[i]->mouseOverMe(newx, newy)) {
			guard[i]->selected = true;
			PanelBotTexId = guard[i]->PanelTexId;
		} else {
			guard[i]->selected = false;
		}
	}
}

void block_convert(float &newx, float &newy, float x, float y)
{
	int tempx, tempy;

	tempx = (int)((map->length / 2.0 + x) / map->blocksize);
	tempy = (int)((map->breadth / 2.0 + y) / map->blocksize);
	newx =
	    -map->length / 2.0 + tempx * map->blocksize + map->blocksize / 2.0;
	newy =
	    -map->breadth / 2.0 + tempy * map->blocksize + map->blocksize / 2.0;
}

void process_events(void)
{
	/* Our SDL event placeholder. */

	if (!hero->Alive)
		Quit(0);

	SDL_Event event;
	/* Grab all the events off the queue. */
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYUP:
			{
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					{
						Bool_Cam_MvyN = false;
						break;
					}
				case SDLK_DOWN:
					{
						Bool_Cam_MvyP = false;
						break;
					}
				case SDLK_LEFT:
					{
						Bool_Cam_MvxN = false;
						break;
					}
				case SDLK_RIGHT:
					{
						Bool_Cam_MvxP = false;
						break;
					}
				case SDLK_SPACE:
					{
						break;
					}
				case SDLK_a:
					{
						Bool_Cam_RotateP = false;
						break;
					}
				case SDLK_d:
					{
						Bool_Cam_RotateN = false;
						break;
					}
				default:
					break;

				}
				break;
			}
		case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					{
						Bool_Cam_MvyN = true;
						break;
					}
				case SDLK_DOWN:
					{
						Bool_Cam_MvyP = true;
						break;
					}
				case SDLK_LEFT:
					{
						Bool_Cam_MvxN = true;
						break;
					}
				case SDLK_RIGHT:
					{
						Bool_Cam_MvxP = true;
						break;
					}
				case SDLK_F2:
					{
						gameMenu = true;
						gameMenuInit = true;
						gameInit = false;
						gamePaused = false;
						gameOver = true;
						break;
					}
				case SDLK_F1:
					{
						showHelp = !showHelp;
						break;
					}
				case SDLK_F3:
					{
						if (panel)
							panel->show_fps = !panel->show_fps;
						break;
					}
				case SDLK_a:
					{
						Bool_Cam_RotateP = true;
						break;
					}
				case SDLK_p:
					{
						gamePaused = !gamePaused;
						break;
					}
				case SDLK_m:
					{
						show_minimap = !show_minimap;
						break;
					}
				case SDLK_d:
					{
						Bool_Cam_RotateN = true;
						break;
					}
				case SDLK_g:
					{
						if (hero->selected)
							hero->gunselected =
							    !hero->gunselected;
						break;
					}
				case SDLK_h:
					{
						hero->selected = true;
						break;
					}
				case SDLK_b:
					{
						if (hero->curx <= -580
						    && hero->cury <= -hres)
							gameOver = true;
						break;
					}
				case SDLK_s:
					{
						break;
					}
				case SDLK_q:
					{
						Quit(0);
						break;
					}
				case SDLK_ESCAPE:
					{
						hero->selected = false;
						break;
					}
				case SDLK_l:
					{
						display_lines = !display_lines;
						break;
					}
				case SDLK_SPACE:
					{
						if (hero->selected) {
							camera->pointx =
							    camera->initx =
							    hero->curx;
							camera->pointz =
							    camera->initz =
							    hero->cury;

							camera->camx =
							    camera->distance *
							    sin(angle) +
							    camera->initx;
							camera->camz =
							    camera->distance *
							    cos(angle) +
							    camera->initz;
						}
						break;
					}
				default:
					break;
				}
				break;
			}

		case SDL_MOUSEMOTION:
			{
				//printf("<%d,%d>\n", event.button.x, event.button.y ); 
				if (event.button.x > hres - 10)
					Bool_Mouse_HotAreaRight = true;
				else
					Bool_Mouse_HotAreaRight = false;

				if (event.button.x < 10)
					Bool_Mouse_HotAreaLeft = true;
				else
					Bool_Mouse_HotAreaLeft = false;

				if (event.button.y > vres - 10)
					Bool_Mouse_HotAreaTop = true;
				else
					Bool_Mouse_HotAreaTop = false;

				if (event.button.y < 10)
					Bool_Mouse_HotAreaBottom = true;
				else
					Bool_Mouse_HotAreaBottom = false;

				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.button == SDL_BUTTON_LEFT) {
					float nx, ny;
					if (minimap->isMouseOver
					    ((int)event.button.x,
					     (int)event.button.y, nx, ny)) {
//                                              printf("NEWX = %f NEWY = %f\n",nx,ny);
						camera->ScrollOver(nx *
								   minimap->xconvfactor,
								   ny *
								   minimap->yconvfactor);
					} else {
						newpickObjects((int)
							       event.button.x,
							       (int)
							       event.button.y);
					}

				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					float nx, ny, px, py;
					if (minimap->isMouseOver
					    ((int)event.button.x,
					     (int)event.button.y, nx, ny)) {
						float fx, fy;

						px = nx * minimap->xconvfactor;
						py = ny * minimap->yconvfactor;

						block_convert(fx, fy, px, py);
						hero->Run(fx, fy, px, py);
					} else {

						float x3, y3;
						camera->ConvertCoordinates((int)
									   event.button.x, (int)
									   event.button.y, x3, y3);

						if (hero->selected) {
							float flag = true;
							for (int i = 0;
							     i < num_guards;
							     i++) {
								if (guard
								    [i]->Alive
								    &&
								    (guard
								     [i]->mouseOverMe
								     (x3, y3))
								    &&
								    (guard[i]->
								     inRange
								     (hero->curx,
								      hero->cury,
								      hero->facingAngle)))
								{
									if (guard[i]->status != DEAD) {
										soundSystem->Play
										    (SOUNDTYPE_SHOOT);
										hero->Attack
										    ();
										guard
										    [i]->Death
										    ();
										printf
										    ("GUARD DESTROYED %d\n",
										     i);
									}
									flag =
									    false;
									break;
								}
								//              if(guard[i]->Alive && (guard[i]->mouseOverMe(x3,y3))&&(!guard[i]->inRange(hero->curx,hero->cury,hero->facingAngle)))
								//              {
								//                      flag = false;
								//              }
							}
							if (flag) {
								float fx, fy;

								block_convert
								    (fx, fy, x3,
								     y3);

								hero->Run(fx,
									  fy,
									  x3,
									  y3);
							}
						}

					}
				}
				if (event.button.button == SDL_BUTTON_WHEELUP) {
					Bool_Cam_Rotate_WheelP = true;
				}
				if (event.button.button == SDL_BUTTON_WHEELDOWN) {
					Bool_Cam_Rotate_WheelN = true;
				}
				break;
			}

		case SDL_QUIT:
			{
				/* Handle quit requests (like Ctrl-c). */
				Quit(0);
				break;
			}
		}
	}
	WorldCamUpdate();
}

void drawObjects(GLenum mode)
{
	float farthestdist = 2 * ((hres/2.0) * (hres/2.0) + (vres/2.0) * (vres/2.0));

	glColor3f(1.0, 1.0, 1.0);

	if ((hero->curx - camera->initx) * (hero->curx - camera->initx) +
	    (hero->cury - camera->initz) * (hero->cury - camera->initz) <=
	    farthestdist) {
		hero->Render();
	}
	for (int i = 0; i < num_guards; i++) {
		if (guard[i]->selected) {
			guard[i]->Render();
		} else {
			if ((guard[i]->curx - camera->initx) * (guard[i]->curx -
								camera->initx) +
			    (guard[i]->cury - camera->initz) * (guard[i]->cury -
								camera->initz)
			    <= farthestdist) {
				guard[i]->Render();
			}
		}
	}
}

void render(void)
{
	/* Just clear the depth buffer, color is overwritten anyway */
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-(hres/2.0), (hres/2.0), -(vres/2.0), (vres/2.0), -hres, hres);
	glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
	glLoadIdentity();	// Reset The Modelview Matrix

	camera->Update();

	//Terrain
	map->Render();
	//printf("MAP CENTER: %f %f\n",camera->pointx,camera->pointz);

	drawObjects(GL_RENDER);

	if (show_minimap)
		panel->Render();

	SDL_GL_SwapBuffers();
}

int main(void)
{
	int width = hres;
	int height = vres;
	int VideoFlags = 0;
	SDL_Surface *MainWindow;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)	// try to initialize SDL video module
	{
		printf("SDL ERROR:%s\n", SDL_GetError());	// report error if it fails
		Quit(0);
	}

	const SDL_VideoInfo *VideoInfo = SDL_GetVideoInfo();	// query SDL for information about our video hardware

	if (VideoInfo == NULL)	// if this query fails
	{
		printf("Failed getting Video Info : %s\n", SDL_GetError());	// report error
		Quit(0);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// tell SDL that the GL drawing is going to be double buffered

	VideoFlags = SDL_OPENGL;

	MainWindow =
	    SDL_SetVideoMode(hres, vres, 16,
			     SDL_FULLSCREEN | SDL_OPENGL | SDL_HWPALETTE);
//      MainWindow = SDL_SetVideoMode(hres, vres, 16, SDL_RESIZABLE | SDL_OPENGL | SDL_HWPALETTE );

	if (MainWindow == NULL)	// if window creation failed
	{
		printf("Failed to Create Window : %s\n", SDL_GetError());	// report error
		Quit(0);
	}
	SDL_WM_SetCaption("SDL Rendering", "SDL Rendering.");
	//SDL_ShowCursor( SDL_DISABLE );

	//cursor kelking...
	cursors[0] = init_system_cursor(arrow);
	SDL_SetCursor(cursors[0]);

	LoadTextures();

	soundSystem = new SoundSystem();
	soundSystem->Initialize();

	Menu *menu = new Menu();
	menu->Initialize();

	//HERO
	//hero = new Hero( -860 , 780 , 270 , 1);
	hero = new Hero(660, 550, 270, 1);

	// CAMERA
	camera = new Camera;

	//PANEL
	panel = new Panel(62);

	//MAP
	map = new Map;
	map->LoadFile();	//only once

	//MINIMAP
	minimap = new MiniMap();
	PanelBotTexId = 65;

	SDL_SetTimer(TIME_INTERVAL, TimerCallback);

	while (1) {
		if (gameMenu) {
			if (gameMenuInit) {
				setup_opengl_menu(width, height);
				gameOver = false;
				gameMenuInit = false;
				menu->Initialize();
				soundSystem->Play(SOUNDTYPE_MENU);
			} else {
				menu->Render();
				menu->MenuProcessEvents();
			}
		} else {
			if (gameInit) {
				setup_opengl_game(width, height);
				Initialize();
				hero->Initialize(-780, 780, 270, 1);
//                              hero->Initialize( 260 , -550 , 270 , 1);
				camera->Initialize();
				map->Initialize();
				gameInit = false;
				soundSystem->Play(SOUNDTYPE_BACKGROUND);
			} else {
				render();
				process_events();
			}
		}
	}

	return 0;
}

Uint32 TimerCallback(unsigned int)
{
	if (!gameMenu) {
		if (!gameOver) {
			if (!gamePaused) {
				hero->NextMove();
				for (int i = 0; i < num_guards; i++) {
					guard[i]->NextMove();
				}
				for (int i = 0; i < num_guards; i++) {
					if (guard[i]->Alive
					    &&
					    (guard[i]->mouseOverMe
					     (hero->destx, hero->desty))
					    && (guard[i]->
						inRange(hero->curx, hero->cury,
							hero->facingAngle))) {
						if (guard[i]->status != DEAD) {
							soundSystem->Play
							    (SOUNDTYPE_SHOOT);
							hero->Attack();
							guard[i]->Death();
							printf
							    ("GUARD DESTROYED %d\n",
							     i);
						}
					}
					if (guard[i]->Alive &&
					    guard[i]->checkIntruder(hero->curx,
								    hero->
								    cury)) {
						soundSystem->
						    Play(SOUNDTYPE_ALARM);
						guard[i]->showbeam = true;
						//hero->status = HERO_DEAD;
						//hero->Death( );
						hero->Stand();
						gameOver = true;
					}
				}
			}
		} else {
			//rotating camera
			Bool_Cam_RotateP = true;
		}
	}
	return 1;
}
