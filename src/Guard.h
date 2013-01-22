#ifndef			__GAME_GUARD_H__
#define			__GAME_GUARD_H__

#include "Md2.h"
#include "RenderPass.h"

typedef enum {
	STANDING,
	RUNNING,
	ATTACKING,
	PATROL,
	DEAD
} GuardStance;

class Guard: public RenderableObject {
 public:
	float curx;
	float cury;

	float destx;
	float desty;

	GuardStance status;

	const MD2 *basemodel;
	AnimObj *md2AnimObj;
	float x, y, z, facingAngle;

	int guardNo;
	float costheta;
	float sintheta;
	bool movingright;
	bool movingup;
	bool selected;
	bool Alive;
	bool showbeam;
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
    float bbox[6];
 public:
	 Guard(char *filename, int texid, float x1, float y1, float x2,
	       float y2, float speed, GLuint fovTexID, float botangle = 0, int no = 0);
	~Guard();
	void Initialize();

	void Run(float x, float y);
	void Stand(float, float);
	void Attack(float x, float y);
	int NextMove(void);
	int Patrol(float, float, float, float);
	void Death(void);
	void FreezeFrame(void);

	int Compute(float &x, float &y);

	void Render(void);
	void Fov(void);

	void Dump(void);

	bool inRange(float, float, float);
	bool checkIntruder(float, float);

	bool mouseOverMe(float x, float y);
    float* GetBB();
    void RenderBBox();
};

#endif				/*      __GAME_GUARD_H__ */
