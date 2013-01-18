#ifndef				__GAME_GLOBALS_H__
#define				__GAME_GLOBALS_H__

//#include "Game.h"
//#include "Menu/menu.h"
#include "SoundSystem.h"
#include <SDL/SDL.h>
#include <assert.h>

#include "platform.h"

#define	MAX_FILENAME_LEN	64

class Game;
class Menu;

extern Game *game;
extern Menu *menu;
extern void fontInit();
extern void drawfontString(const char *, float, float, float, float);
extern SoundSystem *soundSystem;

extern bool gameMenu;
extern bool gameMenuInit;
extern bool gameInit;
extern bool gamePaused;
extern bool gameOver;
extern bool showHelp;
extern bool enable_shaders;

extern int hres;
extern int vres;

extern int curTime;

//cursors
extern SDL_Cursor *cursors[];

#define		SCR2RESX(x)		((x)/640.0 * (hres))
#define		SCR2RESY(y)		((y)/480.0 * (vres))

/* List of Menu Callback functions */
extern void start_game(void);
extern void end_game(void);
extern void Quit(int val);

#endif				/*      __GAME_GLOBALS_H__      */
