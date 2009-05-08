/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2009  Bhanu Chetlapalli
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

	void LoadAvailMaps(MenuItem *singlePlayerItem);

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
