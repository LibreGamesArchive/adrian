#ifndef				__GAME_GLOBALS_H__
#define				__GAME_GLOBALS_H__

#include "Map.h"
#include "Camera.h"
#include "Hero.h"
#include "Guard.h"
#include "MiniMap.h"
#include "Panel.h"
#include "SoundSystem.h"
#include <SDL/SDL.h>

extern Map * map;
extern Camera * camera;
extern Hero * hero;
extern Guard * guard[];
extern int num_guards;
extern float FontTexture[256][2];
extern void block_convert(float &, float &, float, float);
extern void fontInit();
extern void drawfontString( char *, float , float , float , float );
extern MiniMap * minimap;
extern Panel *panel;
extern SoundSystem * soundSystem;

extern bool gameMenu;
extern bool gameMenuInit;
extern bool gameInit;
extern bool gamePaused;
extern bool gameOver;
extern bool showHelp;

extern int PanelBotTexId;

//cursors
extern SDL_Cursor *cursors[4];

#endif			/*	__GAME_GLOBALS_H__	*/
