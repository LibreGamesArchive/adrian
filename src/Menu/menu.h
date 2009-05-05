#ifndef				__MENU_H__
#define				__MENU_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>

#include <GL/gl.h>
#include <SDL/SDL.h>

#include "../texture.h"
#include "menuitem.h"
#include "menupage.h"
#include "../TTFFont.h"
#include "../TextObject.h"

class Menu 
{
 private:
	bool initialized;
	volatile bool lazy_destroy;

	MenuPage *currentMenuPage;

	TTFFont *itemfont;

	GLuint fontTexID;
	Texture *fontTex;

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

	/* Misc textures */
	int num_textures;
	Texture **textures;

 public:
	TTFFont *tahoma;

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
