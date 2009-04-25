#include "menuitem.h"
#include <GL/gl.h>
#include <GL/glu.h>

extern float FontTexture[256][2];

MenuItem::MenuItem(const char *text, float x, float y, void *funcPtr,
		   MenuPagePtr nextMenuPage, bool enabled, float startx,
		   float starty, AnimationType animationType, float fontHeight,
		   float fontWidth)
{
	strncpy(this->text, text, MAX_MENU_STRING_LENGTH);
	this->x = x;
	this->y = y;
	this->funcPtr = (MENU_FUNC) funcPtr;
	this->nextMenuPage = nextMenuPage;
	this->enabled = enabled;
	this->startx = startx;
	this->starty = starty;
	this->animationType = animationType;
	this->fontHeight = fontHeight;
	this->fontWidth = fontWidth;
}

MenuItem::~MenuItem()
{
}

// Accesor Functions
void MenuItem::Disable(bool enabled)
{
	this->enabled = enabled;
}

void MenuItem::setNextMenuPage(MenuPagePtr mpp)
{
	nextMenuPage = mpp;
}

// Rendering Functions
void MenuItem::Render(int method, int offset)
{
	float add_x = (float)10 / (float)128;
	float add_y = (float)15 / (float)128;
	int limit = strlen(text);
	int loop_var = 0;
	const unsigned char *txtarr = (const unsigned char *)text;

	switch (method) {
	case MENUITEM_ORDINARY:
		//normal grey color
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex3f(x, y + fontHeight, -1);
		glVertex3f(x, y, -1);
		glVertex3f(x + fontWidth * 15, y, -1);
		glVertex3f(x + fontWidth * 15, y + fontHeight, -1);
		glEnd();

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.9);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 20);

		glColor4f(.2, .4, .5, 1.5);

		for (loop_var = 0; loop_var < limit; loop_var++) {
			glBegin(GL_POLYGON);
			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth, y + fontHeight, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth + fontWidth, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth + fontWidth,
				   y + fontHeight, 0);
			glEnd();
		}
		//glFlush();    
		break;

	case MENUITEM_HIGHLIGHT:
		//highlighted white color

		glColor3f(0, 1, 0);
		glBegin(GL_POLYGON);
		glVertex3f(x, y + fontHeight, -1);
		glVertex3f(x, y, -1);
		glVertex3f(x + fontWidth * 15, y, -1);
		glVertex3f(x + fontWidth * 15, y + fontHeight, -1);

		glEnd();

		glColor4f(1, 1, 1, 1);
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.9);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 20);

		glColor4f(.2, .4, .5, 1.5);
		for (loop_var = 0; loop_var < limit; loop_var++) {
			glBegin(GL_POLYGON);
			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth, y + fontHeight, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth + fontWidth, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth + fontWidth,
				   y + fontHeight, 0);

			glEnd();
		}

		break;

	case MENUITEM_ANIMATE:
		glColor3f(1, 0, 0);
		glBegin(GL_POLYGON);
		glVertex3f(x, y + fontHeight, -1);
		glVertex3f(x, y, -1);
		glVertex3f(x + fontWidth * 20, y, -1);
		glVertex3f(x + fontWidth * 20, y + fontHeight, -1);
		glEnd();

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.9);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 20);

		glColor4f(.2, .4, .5, 1.5);

		for (loop_var = 0; loop_var < limit; loop_var++) {
			glBegin(GL_POLYGON);
			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth + offset,
				   y + fontHeight, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0],
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth + offset, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth + fontWidth +
				   offset, y, 0);

			glTexCoord2f(FontTexture[txtarr[loop_var]][0] + add_x,
				     FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth + fontWidth +
				   offset, y + fontHeight, 0);
			glEnd();
		}
		break;

	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

}

MenuPagePtr MenuItem::getNextMenuPage(void)
{
	return nextMenuPage;
}

bool MenuItem::isInside(int mousex, int mousey)
{
//      printf("(%d,%d) ( %f,%f )\n",mousex,mousey,x,y );
	if (mousex >= x && mousex <= x + fontWidth * 20) {
		if (768 - mousey >= y && 768 - mousey <= y + fontHeight) {
			return true;
		}
	}
	return false;
}

void MenuItem::Show(void)
{
	if (animationType == ANIMATION_STRAIGHT) {
		xincrement = (x - startx) / 100.0;
		yincrement = (y - starty) / 100.0;
	} else if (animationType == ANIMATION_SPIRAL) {
		xincrement = (x - startx) / 100.0;
		yincrement = (y - starty) / 100.0;
	}

	tmpx = x;
	tmpy = y;
}

void MenuItem::Animate(float animatePercent)
{
	if (animationType == ANIMATION_STRAIGHT) {
		x = tmpx - (xincrement * (100.0 - animatePercent));
		y = tmpy - (yincrement * (100.0 - animatePercent));
	} else if (animationType == ANIMATION_SPIRAL) {
		x = tmpx -
		    ((xincrement * (100.0 - animatePercent))) *
		    sin(animatePercent * 3.6 * 3.142 / 180.0);
		y = tmpy -
		    ((yincrement * (100.0 - animatePercent))) *
		    cos(animatePercent * 3.6 * 3.142 / 180.0);
	}
}

void MenuItem::Hide(void)
{
	xincrement = -1;
	yincrement = -0;
}
