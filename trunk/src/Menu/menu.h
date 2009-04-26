#ifndef				__MENU_H__
#define				__MENU_H__

#include <stdio.h>

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "menuitem.h"
#include "menupage.h"

class Menu 
{
 private:
	bool initialized;
	volatile bool lazy_destroy;

	MenuPage *currentMenuPage;

	GLuint fontTexID;

	void HandleKeyDown(SDL_keysym* keysym);
	void HandleKeyUp(SDL_keysym* keysym);

	void InitFont(void);
	void DestroyFont(void);
	void InitMenuOpenGL(int resx, int resy);

	/* Pages in the Menu */
	MenuPage *parent;
	MenuPage *singlePlayerPage;
	MenuPage *settingsPage;
	MenuPage *videoResPage;
	MenuPage *videoSettingsPage;
	MenuPage *creditsPage;

	/* Animation */
	float linex;
	float increment;

 public:
	Menu(void);
	~Menu();

	float FontTexture[256][2];

	void InitializeMenu(void);
	void DestroyMenu(void);
	void LazyDestroyMenu(void);

	void ProcessEvents(void);
	void Render(void);
	void TimerCallback(void);

	void setCurrentMenuPage(MenuPage *);
};

#endif				/*      __MENU_H__      */
