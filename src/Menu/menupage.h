#ifndef			__MENUPAGE_H__
#define			__MENUPAGE_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
