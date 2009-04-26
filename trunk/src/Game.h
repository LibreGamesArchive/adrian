#ifndef		__GAME_H__
#define		__GAME_H__

#include "Map.h"
#include "Camera.h"
#include "Md2.h"
#include "Hero.h"
#include "Guard.h"
#include "globals.h"
#include "Panel.h"
#include "MiniMap.h"
#include "config.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define		MAX_NUM_TEXTURES	256

class Game
{
	friend class Map;
	friend class MiniMap;
	friend class Camera;
	friend class Building;
	friend class Hero;
	friend class Guard;
	friend class Panel;
 private:
	bool initialized;
	bool started;
	bool gameover;
	bool lazy_destroy;

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
	Map *map;
	Camera *camera;
	Hero *hero;
	Guard *guard[50];
	int num_guards;
	MiniMap *minimap;
	Panel *panel;
	GLuint PanelBotTexId;

	/* Texture and Render information */
	GLuint textureIDs[MAX_NUM_TEXTURES];

	void setupOpenGL(int width, int height);

	void resetVars(void);
	void LoadTextures(void);
	void UnloadTextures(void);
	void WorldCamUpdate(void);
	void newpickObjects(int x, int y);
	void HandleRightClick(int x, int y);
	void drawObjects(GLenum mode);
	void LazyDestroyGame(void);

 public:
	Game(void);
	~Game();

	void addGuard(Guard *g);
	void block_convert(float &newx, float &newy, float x, float y);

	void InitializeGame(void);
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

