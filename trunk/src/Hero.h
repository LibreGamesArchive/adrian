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


#ifndef			__GAME_HERO_H__
#define			__GAME_HERO_H__

#include "Md2.h"

typedef enum {
	HERO_STANDING,
	HERO_RUNNING,
	HERO_ATTACKING,
	HERO_DEAD
} HeroStance;

class Hero:public Md2 {
 public:
	float curx;
	float cury;

	float destx;
	float desty;

	float destx1;
	float desty1;

	HeroStance status;

	float costheta;
	float sintheta;
	bool movingright;
	bool movingup;
	int AttackFrameCount;
	int DeathFrameCount;
	bool gunselected;
	bool Alive;
	float speed;

	float fov;
	float fovAngle;
	float fovInterval;
	float fovStart;
	float fovEnd;
	int fovID;
	int textureID;
	int PanelTexId;
	bool selected;
    float bbox[6];
 public:
	 Hero(float, float, float, float, GLuint fovTexID, GLuint panelTexID);
	~Hero();
	void Initialize(float, float, float, float, GLuint, GLuint);

	void Run(float x, float y, float perfectx, float perfecty);
	void Stand(void);
	void Attack(void);
	void Death();
	int NextMove(void);
	bool mouseOverMe(float x, float y);
    float* GetBB();
	void Fov(void);

	int BuildingOnPath(void);
	int Compute(float &x, float &y, float perfectx, float perfecty);

	void Render(void);

	void Dump(void);
	float Distance(float, float);
	bool CalculateDest(float x1, float y1, float &x2, float &y2);
	bool CheckCurWithBuildings(float &curx, float &cury);
	bool CheckCurWithWalls(float &x, float &y);
    void RenderBBox();

};

#endif				/*      __GAME_HERO_H__ */
