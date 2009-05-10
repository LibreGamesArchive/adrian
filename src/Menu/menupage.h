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


#ifndef			__MENUPAGE_H__
#define			__MENUPAGE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "menuitem.h"

class MenuPage {
 public:
//              static MenuPage * currentMenuPage;

 private:
	int noOfMenuItems;
	MenuItem *menuItemArray[MAX_MENU_ITEMS_PER_PAGE];

	int currentSelection;

	// Animation variables
 private:
	float animatePercent;

 public:
	 MenuPage * parentMenuPage;

	// Constructor Functions
 public:
	 MenuPage(int currentSelection = 0);
	~MenuPage();

	// Accesor Functions
 public:
	 MenuPage * getParentPage(void);
	void setParentPage(MenuPage * parentMenuPage = NULL);

	MenuItem *getCurrentMenuItem(void);
	void setCurrentMenuItem(int currentMenuIndex);

	void addMenuItem(MenuItem * mi);

	// Rendering Functions
 public:
	void Show(void);
	void Animate(void);
	void Hide(void);
	bool isAnimating(void);

	void Render(void);

	void moveDown(void);
	void moveUp(void);

	void accept(void);
	void mouseAccept(int, int);
	void toPreviousMenuPage(void);

	void mouseMove(int mousex, int mousey);

	void Dump(void);
};

#endif				/*      __MENUPAGE_H__  */
