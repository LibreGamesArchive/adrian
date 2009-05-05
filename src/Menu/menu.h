#ifndef				__MENU_H__
#define				__MENU_H__

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

	MenuPage *currentMenuPage;

	TTFFont *itemfont;

	void HandleKeyDown(SDL_keysym* keysym);
	void HandleKeyUp(SDL_keysym* keysym);

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

	void ProcessEvents(void);
	void Render(void);
	void TimerCallback(void);

	void setCurrentMenuPage(MenuPage *);
};

#endif				/*      __MENU_H__      */
