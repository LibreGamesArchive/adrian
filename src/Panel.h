#ifndef __PANEL_H__
#define __PANEL_H__

#include "TTFFont.h"
#include "TextObject.h"

class Panel {
 private:
	TTFFont *font;

	TextObject *fps;
	TextObject *gameOver;
	TextObject *gameOver2;

 public:
	int textureId;
	bool show_fps;
	 Panel(GLuint);
	~Panel();
	void Render();
	void drawBot();
	void drawTips(const char *, const char *);

	void RenderTextObject(TextObject*, float, float, float w = 15, float h = 20);
};

#endif				// __PANEL_H__
