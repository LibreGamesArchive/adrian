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


#include "menu.h"
#include "../texture.h"
#include "../globals.h"
#include "../main.h"

#include <GL/glew.h>
//#include <GL/glu.h>
#include <stdlib.h>

Menu::Menu(void)
{
	initialized = false;
	textures = NULL;
}

Menu::~Menu()
{
}

void Menu::InitMenuOpenGL(int horz_res, int vert_res)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 0.0);
	glViewport(0, 0, horz_res, vert_res);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, horz_res, 0, vert_res, -1, 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void exitGame(void *ign)
{
	exit(0);
}

#define     MAX_NUM_MAPS_IN_MENU    10

void Menu::LoadAvailMaps(MenuItem *singlePlayerItem)
{
	int nextloc = 600;
    int num_files = MAX_NUM_MAPS_IN_MENU;
    char *arr[MAX_NUM_MAPS_IN_MENU];
	// Single Player Menu
	singlePlayerPage = new MenuPage();

    if (get_list_of_files_in_dir(GAME_DATA_PATH"/maps", &num_files, arr) < 0) {
        fprintf(stderr, "Unable to get list of files from maps\n");
        exit(-1);
    }

    for (int i = 0; i < num_files; i++) {
		char buf[256];
		sprintf(buf, "%s/maps/%s/info", GAME_DATA_PATH, arr[i]);
		FILE *f;
		f = fopen(buf, "r");
		if (f) {
			char name[512];
			if (fgets(name, 512, f) > 0) {
				char *n = name + 5;
				for (int j = 0; name[j]; j++) 
					if (name[j] == '\n' || name[j] == '\r')
						name[j] = '\0';

				sprintf(buf, "%s/maps/%s", GAME_DATA_PATH, arr[i]);
				singlePlayerPage->addMenuItem(new
						MenuItem(n, itemfont, 400, nextloc, flag_load_game, (void*)strdup(buf), NULL));
				nextloc -= 100;
			}
			fclose(f);
		}
        free(arr[i]);
	}

/*	MenuItem *startSinglePlayerGameItem =
	    new MenuItem("START GAME", itemfont, 400, 500, flag_load_game, (void*)"maps/default", NULL);
	singlePlayerPage->addMenuItem(startSinglePlayerGameItem);

	MenuItem *loadSinglePlayerGameItem =
	    new MenuItem("LOAD GAME", itemfont, 400, 400, flag_load_game, (void*)"maps/snow", NULL);
	singlePlayerPage->addMenuItem(loadSinglePlayerGameItem);*/

	MenuItem *backSinglePlayerItem =
	    new MenuItem("BACK", itemfont, 400, nextloc, NULL, NULL, NULL);
	singlePlayerPage->addMenuItem(backSinglePlayerItem);

	singlePlayerItem->setNextMenuPage(singlePlayerPage);
	backSinglePlayerItem->setNextMenuPage(parent);


}

void Menu::InitializeMenu(void)
{
	if (initialized)
		return;

	InitMenuOpenGL(hres, vres);

	itemfont = new TTFFont(GAME_DATA_PATH"/fonts/font.ttf", 64, TTF_STYLE_BOLD);

	// Animation
	linex = 0;
	increment = 1;

	// Main Menu
	parent = new MenuPage();

	MenuItem *singlePlayerItem =
	    new MenuItem("PLAY GAME", itemfont, 400, 600, NULL, NULL, NULL, true,
			 0, 600);
	parent->addMenuItem(singlePlayerItem);

	MenuItem *settingsItem =
	    new MenuItem("SETTINGS", itemfont, 400, 500, NULL, NULL, NULL, true, 900, 500);
	parent->addMenuItem(settingsItem);

	// Disabled
	MenuItem *optionsItem =
	    new MenuItem("OPTIONS", itemfont, 400, 400, NULL, NULL, NULL, false, 0, 0);
	parent->addMenuItem(optionsItem);

	MenuItem *creditsItem =
	    new MenuItem("CREDITS", itemfont, 400, 300, NULL, NULL, NULL, true, 400, 0);
	parent->addMenuItem(creditsItem);

	MenuItem *quitItem =
	    new MenuItem("QUIT", itemfont, 400, 200, exitGame, NULL, NULL, true, 400,
			 700);
	parent->addMenuItem(quitItem);

	currentMenuPage = parent;

	/* Basically check the list of available maps first */
	LoadAvailMaps(singlePlayerItem);

	// Multi Player Menu
	settingsPage = new MenuPage();

	MenuItem *changeVideoSettingsItem =
	    new MenuItem("VIDEO", itemfont, 400, 600, NULL, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeVideoSettingsItem);

	MenuItem *changeSoundSettingsItem =
	    new MenuItem("SOUND", itemfont, 400, 500, NULL, NULL, NULL,
			 false, 1000, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeSoundSettingsItem);

	MenuItem *changeKbdSettingsItem =
	    new MenuItem("KEYBOARD", itemfont, 400, 400, NULL, NULL, NULL,
			 false, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeKbdSettingsItem);

	MenuItem *changeMouseSettingsItem =
	    new MenuItem("MOUSE", itemfont, 400, 300, NULL, NULL, NULL,
			 false, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeMouseSettingsItem);

	MenuItem *backMultiPlayerItem =
	    new MenuItem("BACK", itemfont, 400, 200, NULL, NULL, NULL, true, 400,
			 0, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(backMultiPlayerItem);

	settingsItem->setNextMenuPage(settingsPage);
	backMultiPlayerItem->setNextMenuPage(parent);


	// Change video res
	videoResPage = new MenuPage();

	MenuItem *change_res_640_x_480 =
	    new MenuItem("640 X 480", itemfont, 400, 600, NULL, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_640_x_480);
	MenuItem *change_res_800_x_600 =
	    new MenuItem("800 X 600", itemfont, 400, 500, NULL, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_800_x_600);
	MenuItem *change_res_1024_x_768 =
	    new MenuItem("1024 X 768", itemfont, 400, 400, NULL, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_1024_x_768);
	MenuItem *change_res_1600_x_1200 =
	    new MenuItem("1600 X 1200", itemfont, 400, 300, NULL, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_1600_x_1200);
	MenuItem *backVidResItem =
	    new MenuItem("BACK", itemfont, 400, 200, NULL, NULL, NULL, true, 400,
			 0, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(backVidResItem);

	changeVideoSettingsItem->setNextMenuPage(videoResPage);
	backVidResItem->setNextMenuPage(settingsPage);

	// Options Menu

	// Credits Menu
	creditsPage = new MenuPage(3);

	// Disabled Names
	MenuItem *bhanuItem =
	    new MenuItem("BHANU KALYAN", itemfont, 400, 600, NULL, NULL, NULL, false);
	creditsPage->addMenuItem(bhanuItem);

	MenuItem *vamsiItem =
	    new MenuItem("VAMSI KRISHNA", itemfont, 400, 500, NULL, NULL, NULL, false);
	creditsPage->addMenuItem(vamsiItem);

	MenuItem *swamyItem =
	    new MenuItem("SWAMY SUMAN", itemfont, 400, 400, NULL, NULL, NULL, false);
	creditsPage->addMenuItem(swamyItem);

	MenuItem *backCreditsItem = new MenuItem("BACK", itemfont, 400, 300, NULL, NULL, NULL);
	creditsPage->addMenuItem(backCreditsItem);

	creditsItem->setNextMenuPage(creditsPage);
	backCreditsItem->setNextMenuPage(parent);

	num_textures = 1;
	textures = new Texture*[num_textures];
	textures[0] = new Texture(GAME_DATA_PATH"/textures/menu/blood_splatter.jpg");
	textures[0]->Load();

	currentMenuPage->Show();
	currentMenuPage->Animate();

	soundSystem->Load(GAME_DATA_PATH"/wavs/menu.wav", SOUNDTYPE_MENU, -1);
	soundSystem->Load(GAME_DATA_PATH"/wavs/ting.wav", SOUNDTYPE_MENU_TING, 0);

	soundSystem->PlaySound(SOUNDTYPE_MENU);

	initialized = true;
}

/* Called from timer func - so sync with process events and render */
void Menu::DestroyMenu(void)
{
	if (!initialized)
		return;

	initialized = false;

	soundSystem->UnloadAll();
	for (int i = 0; i < num_textures; i++)
		delete textures[i];
	delete[] textures;
	textures = NULL;

	delete itemfont;

	currentMenuPage = NULL;

	delete videoResPage; videoResPage = NULL;

	delete singlePlayerPage; singlePlayerPage = NULL;
	delete settingsPage; settingsPage = NULL;
	delete creditsPage; creditsPage = NULL;

	delete parent; parent = NULL;
}

void Menu::HandleKeyDown(SDL_keysym* keysym)
{
	if (!initialized)
		return;

	if(keysym->sym == SDLK_ESCAPE)
	{
		currentMenuPage->toPreviousMenuPage();
	}

	switch (keysym->sym) {
		case SDLK_UP:
			currentMenuPage->moveUp();
			break;
		case SDLK_DOWN:
			currentMenuPage->moveDown();
			break;
		case SDLK_RETURN:
			currentMenuPage->accept();
			break;
		default:
			break;
	}
}

void Menu::HandleKeyUp(SDL_keysym* keysym)
{
	if (!initialized)
		return;
}

void Menu::ProcessEvents(void)
{
	if (!initialized)
		return;

	SDL_Event event;

	if (currentMenuPage->isAnimating())
		return;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			HandleKeyDown(&event.key.keysym);

		case SDL_KEYUP:
			HandleKeyUp(&event.key.keysym);

		case SDL_MOUSEMOTION:
			currentMenuPage->mouseMove(event.motion.x,event.motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			if( event.button.button == SDL_BUTTON_LEFT )
				currentMenuPage->mouseAccept( event.motion.x , event.motion.y );
			break;

		case SDL_QUIT:
			exit(0);

		default:
			break;
		}
	}
}

void Menu::Render(void)
{
	if (!initialized)
		return;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static int linex1 = rand() * hres;
	glColor3f( 1, 0, 0 );
	glLineWidth(5);
	glBegin(GL_LINES);
		glVertex3f(linex1++ , 0, -1.5);
		glVertex3f(linex1++ , vres, -1.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(linex * 3, 0, -1.5);
		glVertex3f(linex * 3, vres, -1.5);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(0, 200 + linex * 2, -1.5);
		glVertex3f(hres, 200 + linex * 2, -1.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(0, linex * 3, -1.5);
		glVertex3f(hres, linex * 3, -1.5);
	glEnd();

	glColor3f( 1, 1, 1 );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[0]->GetTexID());
	glBegin(GL_QUADS);
		glTexCoord2f(0 ,1);
		glVertex3f(0, 0, -1.6);
		glTexCoord2f(0 ,0);
		glVertex3f(0, vres, -1.6);
		glTexCoord2f(1 ,0);
		glVertex3f(hres, vres, -1.6);
		glTexCoord2f(1 ,1);
		glVertex3f(hres, 0, -1.6);
	glEnd();

	currentMenuPage->Render();
	glFlush();
}

void Menu::TimerCallback(void)
{
	if (!initialized)
		return;

	currentMenuPage->Animate();
    if (linex >= 400)
		increment = -1;
	if (linex <= 0)
		increment = 1;
	linex += increment;
}

void Menu::setCurrentMenuPage(MenuPage *pg)
{
	currentMenuPage = pg;
	currentMenuPage->Show();
}

