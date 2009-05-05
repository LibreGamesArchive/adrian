#ifndef				__MENUITEM_H__
#define				__MENUITEM_H__

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../TextObject.h"

#include "menudefines.h"

typedef void *MenuPagePtr;

typedef void (*MENU_FUNC)(void*);

typedef enum {
	ANIMATION_STRAIGHT,
	ANIMATION_SPIRAL
} AnimationType;

class MenuItem {
 private:
	char text[MAX_MENU_STRING_LENGTH];
	float x;
	float y;

	float fontHeight;
	float fontWidth;

	TextObject *txob;

	MENU_FUNC funcPtr;
	void *funcArg;

	MenuPagePtr nextMenuPage;

	bool enabled;

	AnimationType animationType;

 public:
	 MenuItem(const char *text, TTFFont *itemfont,
			  float x, float y, MENU_FUNC funcPtr = NULL,
			  void *funcArg = NULL, MenuPagePtr nextMenuPage = NULL,
			  bool enabled = true, float startx = 0, float starty = 0,
			  AnimationType animationType = ANIMATION_SPIRAL,
			  float fontHeight = DEFAULT_FONT_HEIGHT,
			  float fontWidth = DEFAULT_FONT_WIDTH);

	~MenuItem();
	void Initialize(void);

	// Animation Variables
 private:
	float xincrement;
	float yincrement;

	float tmpx;
	float tmpy;

	float startx;
	float starty;
	// Accesor Functions
 public:
	void Disable(bool enabled = false);

	void setNextMenuPage(MenuPagePtr mpp = NULL);
	MenuPagePtr getNextMenuPage(void);

	bool isEnabled(void) {
		return enabled;
	} bool isInside(int x, int y);

	void Show(void);
	void Animate(float animatePercent);
	void Hide(void);

	// Rendering Functions
 public:
	void Render(int method, int offset = 0);

	void ExecuteFunc(void) {
		if (funcPtr)
			(*funcPtr) (funcArg);
	}

	MENU_FUNC getFunc(void) {
		return funcPtr;
	}

	void Dump(void) {
		printf("Dumping %s\n", text);
		printf
		    ("X = %f, Y=%f, FontHt = %f , FontWd = %f, funcPtr=%p,NEXTMENUPAGE=%p,Enabled = %u\n",
		     x, y, fontHeight, fontWidth, funcPtr, nextMenuPage,
		     enabled);
	}
};

#endif				/*      __MENUITEM_H__  */
