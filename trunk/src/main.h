#ifndef			__GAME_MAIN_H__
#define			__GAME_MAIN_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "Map.h"
#include "Camera.h"

void flag_load_game(void);
void start_game(void);
void flag_end_game(void);
void end_game(void);

#endif				/*      __GAME_MAIN_H__ */
