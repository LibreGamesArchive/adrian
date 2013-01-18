#include "Menu/menu.h"
#include "Game.h"
#include "SoundSystem.h"
#include <SDL/SDL.h>

void fontInit();
void drawfontString(const char *, float, float, float, float);
Panel *panel;
SDL_Cursor *cursors[4];
SoundSystem *soundSystem = NULL;

Menu *menu = NULL;
Game *game = NULL;

bool enable_shaders = false;

bool showHelp = false;

int curTime = 0;

#if 0
int hres = 640;
int vres = 480;
#else
int hres = 1024;
int vres = 768;
#endif

