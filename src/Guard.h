#ifndef			__GAME_GUARD_H__
#define			__GAME_GUARD_H__

#include "Md2.h"

typedef enum {
	STANDING,
	RUNNING,
	ATTACKING,
	PATROL,
	DEAD
} GuardStance;

class Guard:public Md2 {
 public:
	float curx;
	float cury;

	float destx;
	float desty;

	GuardStance status;

	int guardNo;
	float costheta;
	float sintheta;
	bool movingright;
	bool movingup;
	bool selected;
	bool Alive;
	bool showbeam;
	int DeathFrameCount;
	int AttackFrameCount;
	int PanelTexId;
	float speed;

	//Patrol x1 y1 x2 y2
	float patrolCoords[4];
	bool onwardPatrol;

	float fov;
	float fovAngle;
	float fovInterval;
	float fovStart;
	float fovEnd;
	int fovID;

 public:
	 Guard(char *filename, int texid, float x1, float y1, float x2,
	       float y2, float speed, float botangle = 0, int no = 0);
	~Guard();
	void Initialize();

	int Run(float x, float y);
	int Stand(float, float);
	int Attack(float x, float y);
	int NextMove(void);
	int Patrol(float, float, float, float);
	void Death(void);

	int Compute(float &x, float &y);

	void Render(void);
	void Fov(void);

	void Dump(void);

	bool inRange(float, float, float);
	bool checkIntruder(float, float);

	bool mouseOverMe(float x, float y);

};

#endif				/*      __GAME_GUARD_H__ */
