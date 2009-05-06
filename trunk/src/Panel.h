#ifndef __PANEL_H__
#define __PANEL_H__

#include "TTFFont.h"
#include "TextObject.h"

class Panel {
 private:
	TTFFont *font;

	TextObject *fps;

 public:
	int textureId;
	bool show_fps;
	 Panel(GLuint);
	~Panel();
	void Render();
	void drawBot();
	void drawTips(const char *, const char *);

	void RenderTextObject(TextObject*, float, float);
};

#endif				// __PANEL_H__
