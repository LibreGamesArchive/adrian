#include "Menu.h"
#include <SDL/SDL.h>
#include "globals.h"

Menu::Menu(void)
{
	Initialize();
}

Menu::~Menu()
{
}

void Menu::Initialize(void)
{
	displaymenu = true;
	start = false;
	story = 10;
}

void Menu::MenuProcessEvents(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_q) {
				Quit();
			}
			if (event.key.keysym.sym == SDLK_s) {
				start = true;
				displaymenu = false;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
			if (event.button.button == SDL_BUTTON_LEFT) {
				if ((event.button.x > SCR2RESX(220)
				     && event.button.x < SCR2RESX(420))
				    && ((vres - event.button.y) > SCR2RESY(260)
					&& (vres - event.button.y) < SCR2RESY(330))) {
					start = true;
					displaymenu = false;
				}
				if ((event.button.x > SCR2RESX(220)
				     && event.button.x < SCR2RESX(420))
				    && ((vres - event.button.y) > SCR2RESY(150)
					&& (vres - event.button.y) < SCR2RESY(220))) {
					Quit();
				}
			}
	}
}

void Menu::Quit()
{
	SDL_Quit();
	exit(0);
}

void Menu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2);

	if (displaymenu) {
		glColor3f(1.0, 1.0, 1.0);

		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(SCR2RESX(220), SCR2RESY(330));
		glTexCoord2f(1.0, 1.0);
		glVertex2f(SCR2RESX(420), SCR2RESY(330));
		glTexCoord2f(1.0, 0.5);
		glVertex2f(SCR2RESX(420), SCR2RESY(260));
		glTexCoord2f(0.0, 0.5);
		glVertex2f(SCR2RESX(220), SCR2RESY(260));
		glEnd();

		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.5);
		glVertex2f(SCR2RESX(220), SCR2RESY(220));
		glTexCoord2f(1.0, 0.5);
		glVertex2f(SCR2RESX(420), SCR2RESY(220));
		glTexCoord2f(1.0, 0.0);
		glVertex2f(SCR2RESX(420), SCR2RESY(150));
		glTexCoord2f(0.0, 0.0);
		glVertex2f(SCR2RESX(220), SCR2RESY(150));
		glEnd();

	} else if (start) {
//              Story();
//              if(story <= 0)
		gameMenu = false;
		gameInit = true;
//              story--; 
	}

	glDisable(GL_TEXTURE_2D);
	glFlush();
	SDL_GL_SwapBuffers();
}

void Menu::Story()
{
	glColor4f(1, 0, 0, 1);
	drawfontString("THE STORY BEGINS", 20, 40, 10, 10);
}
