#include "globals.h"
#include "main.h"
#include "Game.h"
#include "Menu/menu.h"
#include "config.h"
#include "cursor.h"

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <signal.h>

bool load_game = false;
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

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char **argv)
#endif
{
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

	return 1;
}

/* Flag the Start of the game */
void flag_load_game(void)
{
	load_game = true;
}

/* Start the game */
void start_game(void)
{
	/* Destroy the menu */
	menu->DestroyMenu();

	/* setup the game */
	game->InitializeGame();

	/* start the game too */
	game->StartGame();

	load_game = false;
}

/* Stop the game and go back to menu */
void flag_end_game(void)
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

