/*
 * ADRIAN - A 3D role playing game
 * Copyright (C) 2003  Bhanu Chetlapalli, Vamsi Krishna, Swamy Suman
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


#include "Panel.h"
#include "globals.h"
#include "Game.h"

//#include <GL/glu.h>
#include <GL/glew.h>

#include <time.h>

Panel::Panel(GLuint texid)
{
	textureId = texid;
	show_fps = false;

	font = new TTFFont(GAME_DATA_PATH"/fonts/font.ttf", 64, 0);

	fps = new TextObject(font);
	gameOver = NULL;
	gameOver2 = NULL;
}

Panel::~Panel()
{
	if (gameOver2) delete gameOver2;
	if (gameOver) delete gameOver;
	delete fps;
	delete font;
}

void Panel::RenderTextObject(TextObject *tob, float x, float y, float w, float h)
{
	float x1 = SCR2RESX(x);
	float y1 = SCR2RESY(y);

	float x2 = x1 + tob->getLen() * SCR2RESX(w);
	float y2 = y1 + SCR2RESY(h);

	glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tob->texid);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0);
		glVertex2f(x2, y2);
		glTexCoord2f(1, 1);
		glVertex2f(x2, y1);
		glTexCoord2f(0, 1);
		glVertex2f(x1, y1);
		glTexCoord2f(0, 0);
		glVertex2f(x1, y2);
	glEnd();
}

void Panel::Render()
{
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, hres, 0, vres);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1);
	glVertex2f(SCR2RESX(0), SCR2RESY(0));
	glTexCoord2f(10, 1);
	glVertex2f(SCR2RESX(640), SCR2RESY(0));
	glTexCoord2f(10, 0);
	glVertex2f(SCR2RESX(640), SCR2RESY(50));
	glTexCoord2f(0, 0);
	glVertex2f(SCR2RESX(0), SCR2RESY(50));
	glEnd();

	glColor3f(1, 1, 1);
	if (showHelp) {
		drawfontString("F2 : QUIT GAME", 20, 450, 15, 15);
		drawfontString("P  : PAUSE GAME", 20, 430, 15, 15);
		drawfontString("A  : ROTATE CAMERA ANTICLOCKWISE", 20, 410, 15,
			       15);
		drawfontString("D  : ROTATE CAMERA CLOCKWISE", 20, 390, 15, 15);
		drawfontString("M  : TOGGLE PANEL", 20, 370, 15, 15);
		drawfontString("B  : BOMB", 20, 350, 15, 15);
		drawfontString("H  : SELECT HERO", 20, 330, 15, 15);
		drawfontString("SPACE  : BRINGS HERO TO SCREEEN CENTER", 20,
			       310, 15, 15);
		drawfontString("Q  : QUIT", 20, 290, 15, 15);
		drawfontString(" ", 20, 270, 15, 15);
		drawfontString("LEFT CLICK NEAR THE LEG TO SELECT", 20, 250, 15,
			       15);
		drawfontString("RIGHT CLICK TO MOVE", 20, 230, 15, 15);
		drawfontString("RIGHT CLICK ON THE RED CIRCLE", 20, 210, 15,
			       15);
		drawfontString("          BELOW ENEMY TO KILL", 20, 190, 15,
			       15);
		drawfontString("OBJECTIVE: ", 20, 170, 15, 15);
		drawfontString("DESTROY THE EVIL SCIENTISTS CONTROL ROOM", 20,
			       150, 15, 15);
		drawfontString("AT THE TOP RIGHT CORNER OF THE MAP BY", 20, 130,
			       15, 15);
		drawfontString("PLACING A BOMB UNDER HIS HOUSE", 20, 110, 15,
			       15);
	}
	glDisable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);
	drawfontString("F1:HELP", 500, 450, 15, 15);

	static int fpscounter = 0;
	static unsigned long int tmptime = 0;
    static char fps_str[16];

	fpscounter += 1;
	int gap = time(NULL) - tmptime;
	if (gap) {
		tmptime += gap;
#ifdef WIN32
		sprintf_s(fps_str, 16, "FPS: %.1f", ((float)fpscounter)/(float)gap);
#else
		snprintf(fps_str, 16, "FPS: %.1f", ((float)fpscounter)/(float)gap);
#endif
		fpscounter = 0;
	}
	if (show_fps) {
		fps->setText(fps_str);
		RenderTextObject(fps, 300, 450);
//drawfontString(fps_str, 300, 450, 15, 15);
	}

	if (!game->gameover) {
		drawBot();
	} else {
		if (gameOver == NULL) {
			gameOver = new TextObject(font);
			gameOver->setColor(1, 0, 0);
			gameOver->setText("GAME OVER");
			gameOver2 = new TextObject(font);
			gameOver2->setText("Press F2 to exit to Main Menu");
		}
		
		RenderTextObject(gameOver, 200, 220, 40, 50);
		RenderTextObject(gameOver2, 100, 280, 20, 30);
		//drawTips("GAME OVER", "PRESS O TO MENU");
	}
	game->minimap->Render();
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);

}

void Panel::drawBot()
{
	switch (game->PanelBotTexId) {
		//hero 65 hero
		//cheeta 66     animal1
		//man 67 animal
		//dino 68 animal2
	case 65:
		drawTips("RIGHT CLICK ON ENEMYS LEG", "TO KILL IT");
		break;
	case 66:
		drawTips("MUTANT MAN", "AS FAST AS HERO");
		break;
	case 67:
		drawTips("THE CORPSE", "SLOWER THAN HERO");
		break;
	case 68:
		drawTips("THE UNDEAD", "VERY SLOW MUTANT");
		break;
	case 69:
		drawTips("MUTANT CHEETA", "ITS FAST ENOUGH TO BE KILLED");
		break;
	case 70:
		drawTips("MUTANT LIZARD", "SLOWER THAN MUTANT MAN");
		break;
	case 71:
		drawTips("RIPPER", "VERY FAST TRY SNEAKING PAST IT");
		break;
	default:
		break;
	}

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc(GL_GEQUAL, 0.01);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, game->PanelBotTexId);

	glColor4f(1, 1, 1, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(0, 1);
	glVertex2f(SCR2RESX(0), SCR2RESY(10));
	glTexCoord2f(1, 1);
	glVertex2f(SCR2RESX(100), SCR2RESY(10));
	glTexCoord2f(1, 0);
	glVertex2f(SCR2RESX(100), SCR2RESY(110));
	glTexCoord2f(0, 0);
	glVertex2f(SCR2RESX(0), SCR2RESY(110));
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

void Panel::drawTips(const char *str1, const char *str2)
{
	drawfontString(str1, 120, 20, 15, 15);
	drawfontString(str2, 120, 0, 15, 15);
}
