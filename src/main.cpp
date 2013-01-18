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


#include "globals.h"
#include "main.h"
#include "Game.h"
#include "Menu/menu.h"
#include "config.h"
#include "cursor.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
//#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <signal.h>

#include <getopt.h>

bool load_game = false;
const char *next_game_map = NULL;
bool end_game_show_menu = false;

#define			TIME_INTERVAL			300
extern Uint32 TimerCallback(unsigned int);

extern SDL_Cursor *init_system_cursor(const char *image[]);

void Quit(int val)
{
	SDL_SetTimer(TIME_INTERVAL, NULL);
	SDL_Quit();
	exit(val);
}

void Usage(void)
{
	fprintf(stderr, "./adrian [options]\n");
	fprintf(stderr, "\t-S : Enables Shaders\n");
	fprintf(stderr, "\t-s : Disables Sound\n");
	fprintf(stderr, "\t-h : Prints this help message\n");
}

int main(int argc, char *argv[])
{
	int option_idx = 0;
	while (1) {
		int c;
		const static struct option longopts[] = {
			{ "nosound", no_argument, NULL, 's'},
			{ "enable-shaders", no_argument, NULL, 'S'},
			{ "help", no_argument, NULL, 'h'},
			{ NULL, 0, 0, 0}
		};

		c = getopt_long(argc, argv, "sSh",
				longopts, &option_idx);

		if (c == -1)
			break;

		switch (c) {
			case 's':
				SoundSystem::flag_no_sound = 1;
				break;
			case 'S':
				enable_shaders = true;
				break;
			case 'h':
			default:
				Usage();
				return -1;
		}
	}

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

//	MainWindow =
//	    SDL_SetVideoMode(hres, vres, 16,
//			     SDL_FULLSCREEN | SDL_OPENGL | SDL_HWPALETTE);
      MainWindow = SDL_SetVideoMode(hres, vres, 16, SDL_RESIZABLE | SDL_OPENGL | SDL_HWPALETTE );

	if (MainWindow == NULL)	// if window creation failed
	{
		printf("Failed to Create Window : %s\n", SDL_GetError());	// report error
		Quit(0);
	}
	SDL_WM_SetCaption("A D R I A N", "A D R I A N");
	//SDL_ShowCursor( SDL_DISABLE );

	//cursor kelking...
	cursors[0] = init_system_cursor(arrow);
	SDL_SetCursor(cursors[0]);

	soundSystem = new SoundSystem();
	soundSystem->Initialize();

	game = new Game();

	menu = new Menu();
	menu->InitializeMenu();

	SDL_SetTimer(TIME_INTERVAL, TimerCallback);

	while (1) {
		menu->Render();
		game->Render();
		SDL_GL_SwapBuffers();
		menu->ProcessEvents();
		game->ProcessEvents();

		if (load_game)
			start_game();

		if (end_game_show_menu)
			end_game();
	}

	return 0;
}

Uint32 TimerCallback(unsigned int)
{
	menu->TimerCallback();
	game->TimerCallback();

	curTime++;

	return 1;
}

/* Flag the Start of the game */
void flag_load_game(void *ptr)
{
	load_game = true;
	next_game_map = (const char *)ptr;
}

/* Start the game */
void start_game(void)
{
	/* Destroy the menu */
	menu->DestroyMenu();

	/* setup the game */
	game->InitializeGame(next_game_map);

	/* start the game too */
	game->StartGame();

	load_game = false;
}

/* Stop the game and go back to menu */
void flag_end_game(void *ign)
{
	end_game_show_menu = true;
}

/* Stop the game and go back to menu */
void end_game(void)
{
	/* Stop playing it */
	game->EndGame();

	/* destroy the game */
	game->DestroyGame();

	/* Init the menu */
	menu->InitializeMenu();

	end_game_show_menu = false;
}

