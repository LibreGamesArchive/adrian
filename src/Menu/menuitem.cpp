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


#include "menuitem.h"
/*#include <GL/gl.h>
#include <GL/glu.h>*/

#include "menu.h"
#include "../globals.h"

MenuItem::MenuItem(const char *text, TTFFont *itemfont,
				   float x, float y, MENU_FUNC funcPtr, void *funcArg,
				   MenuPagePtr nextMenuPage, bool enabled, float startx,
				   float starty, AnimationType animationType, float fontHeight,
				   float fontWidth)
{
	strncpy(this->text, text, MAX_MENU_STRING_LENGTH);
	this->x = (x*hres)/1024;
	this->y = (y*vres)/768;
	this->funcPtr = funcPtr;
	this->funcArg = funcArg;
	this->nextMenuPage = nextMenuPage;
	this->enabled = enabled;
	this->startx = (startx*hres)/1024;
	this->starty = (starty*vres)/768;
	this->animationType = animationType;
	this->fontHeight = fontHeight;
	this->fontWidth = fontWidth;

	txob = new TextObject(itemfont);
	txob->setColor(1.0, 0, 0);
	txob->setText(text);
}

MenuItem::~MenuItem()
{
	delete txob;
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
//	float add_x = (float)10 / (float)128;
//	float add_y = (float)15 / (float)128;
	int limit = strlen(text);
//	int loop_var = 0;
//	const unsigned char *txtarr = (const unsigned char *)text;

	switch (method) {
	case MENUITEM_ORDINARY:

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.0);

		//normal grey color
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txob->texid);
		glColor3f(1, 1, 1);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(x, y + fontHeight, -1);
			glTexCoord2f(0, txob->ty);
			glVertex3f(x, y, -1);
			glTexCoord2f(txob->tx, txob->ty);
			glVertex3f(x + fontWidth * limit, y, -1);
			glTexCoord2f(txob->tx, 0);
			glVertex3f(x + fontWidth * limit, y + fontHeight, -1);
		glEnd();

//		glEnable(GL_ALPHA_TEST);
//		glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//		glAlphaFunc(GL_GEQUAL, 0.9);
//		glBindTexture(GL_TEXTURE_2D, txob->texid);

//		glColor4f(.2, .4, .5, 1.5);

/*		glBegin(GL_POLYGON);
		glTexCoord2f(1,0);
//		glVertex3f(x + limit*fontWidth, y, 0);
		glVertex3f(x + 200, y, 0);
		glTexCoord2f(1,1);
//		glVertex3f(x + limit*fontWidth, y + fontHeight, 0);
		glVertex3f(x + 200, y + 50, 0);
		glTexCoord2f(0,1);
		glVertex3f(x, y + 50, 0);
//		glVertex3f(x, y + fontHeight, 0);
		glTexCoord2f(0,0);
		glVertex3f(x, y, 0);
		glEnd();
*/
/*		for (loop_var = 0; loop_var < limit; loop_var++) {
			glBegin(GL_POLYGON);
			glTexCoord2f(menu->FontTexture[txtarr[loop_var]][0],
				     menu->FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth, y + fontHeight, 0);

			glTexCoord2f(menu->FontTexture[txtarr[loop_var]][0],
				     menu->FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth, y, 0);

			glTexCoord2f(menu->FontTexture[txtarr[loop_var]][0] + add_x,
				     menu->FontTexture[txtarr[loop_var]][1] + add_y);
			glVertex3f(x + loop_var * fontWidth + fontWidth, y, 0);

			glTexCoord2f(menu->FontTexture[txtarr[loop_var]][0] + add_x,
				     menu->FontTexture[txtarr[loop_var]][1]);
			glVertex3f(x + loop_var * fontWidth + fontWidth,
				   y + fontHeight, 0);
			glEnd();
		}*/
		//glFlush();    
		break;

	case MENUITEM_HIGHLIGHT:
		//highlighted white color

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.0);

		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txob->texid);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(x+20, y + fontHeight, -1);
			glTexCoord2f(0, txob->ty);
			glVertex3f(x+20, y, -1);
			glTexCoord2f(txob->tx, txob->ty);
			glVertex3f(x+20 + fontWidth * limit, y, -1);
			glTexCoord2f(txob->tx, 0);
			glVertex3f(x+20 + fontWidth * limit, y + fontHeight, -1);
		glEnd();


		break;

	case MENUITEM_ANIMATE:
		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glAlphaFunc(GL_GEQUAL, 0.0);

		glColor3f(1, 1, 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, txob->texid);
		glBegin(GL_POLYGON);
			glTexCoord2f(0,0);
			glVertex3f(x + offset, y + fontHeight, -1);
			glTexCoord2f(0, txob->ty);
			glVertex3f(x + offset, y, -1);
			glTexCoord2f(txob->tx, txob->ty);
			glVertex3f(x + offset + fontWidth * limit, y, -1);
			glTexCoord2f(txob->tx, 0);
			glVertex3f(x + offset + fontWidth * limit, y + fontHeight, -1);
		glEnd();

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
		if (vres - mousey >= y && vres - mousey <= y + fontHeight) {
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
