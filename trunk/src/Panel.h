#ifndef __PANEL_H__
#define __PANEL_H__

class Panel {
 public:
	int textureId;
	bool show_fps;
	 Panel(int);
	~Panel();
	void Render();
	void drawBot();
	void drawTips(const char *, const char *);
};

#endif				// __PANEL_H__
