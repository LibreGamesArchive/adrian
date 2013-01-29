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


#ifndef		__GAME_H__
#define		__GAME_H__

#include "RenderPass.h"
#include "Map.h"
#include "Camera.h"
#include "Md2.h"
#include "Hero.h"
#include "Guard.h"
#include "globals.h"
#include "Panel.h"
#include "MiniMap.h"
#include "config.h"
#include "TTFFont.h"

#define	MAX_NUM_GUARDS	50

/*#include <GL/gl.h>
#include <GL/glu.h>*/
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Game
{
	friend class Map;
	friend class MiniMap;
	friend class Camera;
	friend class Building;
	friend class Hero;
	friend class Guard;
	friend class Sprite;
	friend class Panel;
 private:
	bool started;
	bool gameover;
	bool gamelost;

	/* Key event variables */
	bool Bool_Cam_RotateP;
	bool Bool_Cam_RotateN;
	bool Bool_Cam_Rotate_WheelP;
	bool Bool_Cam_Rotate_WheelN;
	bool Bool_Cam_MvyP;
	bool Bool_Cam_MvyN;
	bool Bool_Cam_MvxP;
	bool Bool_Cam_MvxN;
	bool Bool_Mouse_HotAreaRight;
	bool Bool_Mouse_HotAreaLeft;
	bool Bool_Mouse_HotAreaTop;
	bool Bool_Mouse_HotAreaBottom;

	float angle;

	/* Debugging Internal variables */
	bool display_lines;
	bool show_minimap;

	/* Guard & Hero informations */
	Camera *camera;
	Hero *hero;
	Guard *guard[MAX_NUM_GUARDS];
	int num_guards;
	MiniMap *minimap;
	Panel *panel;
	GLuint PanelBotTexId;

    //scene handler for game rendering.
    SceneComposer *scene;

	void setupOpenGL(int width, int height);

	void resetVars(void);
	void WorldCamUpdate(void);
	void newpickObjects(int x, int y);
	void HandleRightClick(int x, int y);	

 public:
	bool initialized;
	Map *map;

	/* Cheat Codes */
	bool cheat_code_invisible_enabled;
	bool cheat_code_nowalls_enabled;

	Game(void);
	~Game();

	void addGuard(Guard *g);
	void block_convert(float &newx, float &newy, float x, float y);

	void InitializeGame(const char *gamefile);
	void DestroyGame(void);

	void ProcessEvents(void);
	void Render(void);
	void TimerCallback(void);

	void StartGame(void);
	void PauseGame(void);
	void UnpauseGame(void);
	void EndGame(void);
};

#endif	/*	__GAME_H__	*/

