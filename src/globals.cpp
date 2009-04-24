#include "Map.h"
#include "Camera.h"
#include "Hero.h"
#include "Guard.h"
#include "MiniMap.h"
#include "Panel.h"
#include "SoundSystem.h"
#include <SDL/SDL.h>

Map *map;
Camera *camera;
Hero *hero;
Guard *guard[50];
int num_guards = 0;
void block_convert(float &, float &, float, float);
float FontTexture[256][2];
void fontInit();
void drawfontString(const char *, float, float, float, float);
MiniMap *minimap;
Panel *panel;
SDL_Cursor cursors[4];
SoundSystem *soundSystem = NULL;

bool gameMenu = true;
bool gameMenuInit = true;
bool gameInit = false;
bool gamePaused = false;
bool gameOver = true;
int PanelBotTexId = 0;
bool showHelp = false;
