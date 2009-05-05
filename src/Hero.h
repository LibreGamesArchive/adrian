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

 public:
	 Hero(float, float, float, float);
	~Hero();
	void Initialize(float, float, float, float);

	void Run(float x, float y, float perfectx, float perfecty);
	void Stand(void);
	void Attack(void);
	void Death();
	int NextMove(void);
	bool mouseOverMe(float x, float y);
	void Fov(void);

	int BuildingOnPath(void);
	int Compute(float &x, float &y, float perfectx, float perfecty);

	void Render(void);

	void Dump(void);
	float Distance(float, float);
	bool CalculateDest(float x1, float y1, float &x2, float &y2);
	bool CheckCurWithBuildings(float &curx, float &cury);
	bool CheckCurWithWalls(float &x, float &y);

};

#endif				/*      __GAME_HERO_H__ */
