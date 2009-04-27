#include "Game.h"
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <signal.h>

extern SDL_Cursor *init_system_cursor(const char *image[]);

Game::Game(void)
{
	initialized = false;
	lazy_destroy = false;

	hero = NULL;
	camera = NULL;
	map = NULL;
	minimap = NULL;
	num_guards = 0;
	memset(guard, 0, sizeof(guard));

	resetVars();
}

Game::~Game()
{
}

/* This should contain ONLY LEGAL RESET VALUES!!!
 * For example **NEVER** NULL a pointer in this!! */
void Game::resetVars(void)
{
	started = false;
	gameover = false;

//	width = hres;
//	height = vres;

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

	angle = START_ANGLE;
}

void Game::setupOpenGL(int width, int height)
{
	if (!initialized)
		return;

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

void Game::InitializeGame(void)
{
	resetVars();

	/* Basic setup of OpenGL */
	setupOpenGL(hres, vres);
	
	/* Create the structures */
	camera = new Camera;
	map = new Map;
	map->LoadFile();	//only once
	panel = new Panel(62);
	hero = new Hero(660, 550, 270, 1);
	minimap = new MiniMap();
	PanelBotTexId = 65;

	/* font init requires a TGA loaded by map */
	fontInit();

	hero->Initialize(-780, 780, 270, 1);
	camera->Initialize();
	map->Initialize();
	soundSystem->Play(SOUNDTYPE_BACKGROUND);

	initialized = true;
}

void Game::DestroyGame(void)
{
	initialized = false;

	lazy_destroy = true;

	soundSystem->HaltAllChannels();

	resetVars();
}

void Game::LazyDestroyGame(void)
{
	if (!lazy_destroy)
		return;

	map->Destroy();

	delete minimap;
	delete panel;
	delete hero;
	for (int i = 0; i < num_guards; i++)
		delete guard[i];
	delete map;
	delete camera;

	hero = NULL;
	camera = NULL;
	map = NULL;
	minimap = NULL;
	num_guards = 0;
	memset(guard, 0, sizeof(guard));

	// destroy font?

	lazy_destroy = false;
}

void Game::addGuard(Guard *g)
{
	guard[num_guards++] = g;
}

void Game::WorldCamUpdate(void)
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

void Game::newpickObjects(int x, int y)
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

void Game::block_convert(float &newx, float &newy, float x, float y)
{
	int tempx, tempy;

	tempx = (int)((map->length / 2.0 + x) / map->blocksize);
	tempy = (int)((map->breadth / 2.0 + y) / map->blocksize);
	newx =
	    -map->length / 2.0 + tempx * map->blocksize + map->blocksize / 2.0;
	newy =
	    -map->breadth / 2.0 + tempy * map->blocksize + map->blocksize / 2.0;
}

void Game::StartGame(void)
{
	started = true;
}

void Game::PauseGame(void)
{
	started = false;
}

void Game::UnpauseGame(void)
{
	started = true;
}

void Game::EndGame(void)
{
	started = false;
}

void Game::ProcessEvents(void)
{
	if (!initialized)
		return;

	if (!hero->Alive)
		Quit(0);

	SDL_Event event;
	/* Grab all the events off the queue. */
	while (SDL_PollEvent(&event)) {
		/* These events are handled even if the game is finished */
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
						end_game();
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
//						gamePaused = !gamePaused;
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
//						if (hero->curx <= -580
//						    && hero->cury <= -hres)
//							gameOver = true;
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
							camera->pointx = camera->initx = hero->curx;
							camera->pointz = camera->initz = hero->cury;

							camera->camx = camera->distance * sin(angle) + camera->initx;
							camera->camz = camera->distance * cos(angle) + camera->initz;
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
						camera->ScrollOver(nx *
								   minimap->xconvfactor,
								   ny *
								   minimap->yconvfactor);
					} else {
						newpickObjects((int)event.button.x,
									   (int)event.button.y);
					}

				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					HandleRightClick(event.button.x, event.button.y);
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

	if (lazy_destroy)
		LazyDestroyGame();
}

void Game::drawObjects(GLenum mode)
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

void Game::Render(void)
{
	if (!initialized)
		return;

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

	map->TransparentRender();

	if (show_minimap)
		panel->Render();
}

void Game::HandleRightClick(int x, int y)
{
	if (!started)
		return;

	float nx, ny, px, py;
	if (minimap->isMouseOver(x, y, nx, ny)) {
		float fx, fy;

		px = nx * minimap->xconvfactor;
		py = ny * minimap->yconvfactor;

		block_convert(fx, fy, px, py);
		hero->Run(fx, fy, px, py);
	} else {
		float x3, y3;
		camera->ConvertCoordinates(x, y, x3, y3);

		if (hero->selected) {
			/* Flag indicates if user wants Hero to run. false means attack */
			float flag = true;
			for (int i = 0; i < num_guards; i++) {
				if (guard[i]->Alive	&&
					guard[i]->mouseOverMe(x3, y3)	&&
					guard[i]->inRange(hero->curx, hero->cury, hero->facingAngle)) 
				{
					if (guard[i]->status != DEAD) {
						soundSystem->Play(SOUNDTYPE_SHOOT);
						hero->Attack();
						guard[i]->Death();
						printf("GUARD DESTROYED %d\n", i);
					}
					flag = false;
					break;
				}
			}
			if (flag) {
				float fx, fy;

				block_convert(fx, fy, x3, y3);
				hero->Run(fx, fy, x3, y3);
			}
		}
	}
}

void Game::TimerCallback(void)
{
	if (!initialized)
		return;

	if (!started)
		return;

	/* Move the hero & creatures */
	hero->NextMove();
	for (int i = 0; i < num_guards; i++)
		guard[i]->NextMove();

	/* Check if hero killed a guard or stepped on anyone's toes :P */
	for (int i = 0; i < num_guards; i++) {
		if (guard[i]->Alive &&
			guard[i]->mouseOverMe(hero->destx, hero->desty) &&
			guard[i]->inRange(hero->curx, hero->cury, hero->facingAngle))
		{
			if (guard[i]->status != DEAD) {
				soundSystem->Play(SOUNDTYPE_SHOOT);
				hero->Attack();
				guard[i]->Death();
				printf("GUARD DESTROYED %d\n", i);
			}
		}
		if (guard[i]->Alive &&
			guard[i]->checkIntruder(hero->curx, hero->cury))
		{
			soundSystem->Play(SOUNDTYPE_ALARM);
			guard[i]->showbeam = true;
			hero->Stand();
			started = false;
			gameover = true;
		}
	}

}

