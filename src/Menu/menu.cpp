#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "menu.h"
#include "../tga.h"
#include "../globals.h"

Menu::Menu(void)
{
	initialized = false;
	lazy_destroy = false;
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

void Menu::InitFont(void)
{
	int ret;

//	glGenTextures(1, &fontTexID);
	fontTexID = 51;
	if ((ret = loadTGA("maps/font.tga", fontTexID)) != 0) {
		printf("Could not load textures: %d\n", ret);
		exit(1);
	}
	if ((ret = loadTGA("maps/blood_splatter.tga", 52)) != 0) {
		printf("Could not load textures: %d\n", ret);
		exit(1);
	}


	FontTexture[46][0] = .625;
	FontTexture[46][1] = .625;

	FontTexture[63][0] = .742;
	FontTexture[63][1] = .875;

	FontTexture[35][0] = .25;
	FontTexture[35][1] = .5;

	FontTexture[32][0] = .875;
	FontTexture[32][1] = .875;

	FontTexture[48][0] = .875;
	FontTexture[48][1] = .625;

	FontTexture[49][0] = 0;
	FontTexture[49][1] = .75;

	FontTexture[50][0] = .117;
	FontTexture[50][1] = .75;

	FontTexture[51][0] = .25;
	FontTexture[51][1] = .75;

	FontTexture[52][0] = .367;
	FontTexture[52][1] = .75;

	FontTexture[53][0] = .5;
	FontTexture[53][1] = .75;

	FontTexture[54][0] = .625;
	FontTexture[54][1] = .75;

	FontTexture[55][0] = .742;
	FontTexture[55][1] = .75;

	FontTexture[56][0] = .875;
	FontTexture[56][1] = .75;

	FontTexture[57][0] = 0;
	FontTexture[57][1] = .875;

	FontTexture[58][0] = .125;
	FontTexture[58][1] = .875;

	FontTexture[65][0] = 0;
	FontTexture[65][1] = 0;

	FontTexture[66][0] = .117;
	FontTexture[66][1] = 0;

	FontTexture[67][0] = .25;
	FontTexture[67][1] = 0;

	FontTexture[68][0] = .367;
	FontTexture[68][1] = 0;

	FontTexture[69][0] = .5;
	FontTexture[69][1] = 0;

	FontTexture[70][0] = .625;
	FontTexture[70][1] = 0;

	FontTexture[71][0] = .742;
	FontTexture[71][1] = 0;

	FontTexture[72][0] = .875;
	FontTexture[72][1] = 0;

	FontTexture[73][0] = 0;
	FontTexture[73][1] = .125;

	FontTexture[74][0] = .117;
	FontTexture[74][1] = .125;

	FontTexture[75][0] = .25;
	FontTexture[75][1] = .125;

	FontTexture[76][0] = .367;
	FontTexture[76][1] = .125;

	FontTexture[77][0] = .5;
	FontTexture[77][1] = .125;

	FontTexture[78][0] = .625;
	FontTexture[78][1] = .125;

	FontTexture[79][0] = .742;
	FontTexture[79][1] = .125;

	FontTexture[80][0] = .875;
	FontTexture[80][1] = .125;

	FontTexture[81][0] = 0;
	FontTexture[81][1] = .25;
	FontTexture[82][0] = .117;
	FontTexture[82][1] = .25;
	FontTexture[83][0] = .25;
	FontTexture[83][1] = .25;
	FontTexture[84][0] = .367;
	FontTexture[84][1] = .25;
	FontTexture[85][0] = .5;
	FontTexture[85][1] = .25;
	FontTexture[86][0] = .625;
	FontTexture[86][1] = .25;
	FontTexture[87][0] = .742;
	FontTexture[87][1] = .25;
	FontTexture[88][0] = .875;
	FontTexture[88][1] = .25;
	FontTexture[89][0] = 0;
	FontTexture[89][1] = .375;
	FontTexture[90][0] = .117;
	FontTexture[90][1] = .375;

}

void Menu::DestroyFont(void)
{
	glDeleteTextures(1, &fontTexID);
}

void exitGame(void)
{
	exit(0);
}

void Menu::InitializeMenu(void)
{
	if (initialized)
		return;

	// Animation
	linex = 0;
	increment = 1;

	// Main Menu
	parent = new MenuPage();

	MenuItem *singlePlayerItem =
	    new MenuItem("PLAY GAME", 400, 600, NULL, NULL, true,
			 0, 600);
	parent->addMenuItem(singlePlayerItem);

	MenuItem *settingsItem =
	    new MenuItem("SETTINGS", 400, 500, NULL, NULL, true, 900, 500);
	parent->addMenuItem(settingsItem);

	// Disabled
	MenuItem *optionsItem =
	    new MenuItem("OPTIONS", 400, 400, NULL, NULL, false, 0, 0);
	parent->addMenuItem(optionsItem);

	MenuItem *creditsItem =
	    new MenuItem("CREDITS", 400, 300, NULL, NULL, true, 400, 0);
	parent->addMenuItem(creditsItem);

	MenuItem *quitItem =
	    new MenuItem("QUIT", 400, 200, exitGame, NULL, true, 400,
			 700);
	parent->addMenuItem(quitItem);

	currentMenuPage = parent;

	// Single Player Menu
	singlePlayerPage = new MenuPage();

	MenuItem *startSinglePlayerGameItem =
	    new MenuItem("START GAME", 400, 500, start_game, NULL);
	singlePlayerPage->addMenuItem(startSinglePlayerGameItem);

	MenuItem *loadSinglePlayerGameItem =
	    new MenuItem("LOAD GAME", 400, 400, NULL, NULL, false);
	singlePlayerPage->addMenuItem(loadSinglePlayerGameItem);

	MenuItem *backSinglePlayerItem =
	    new MenuItem("BACK", 400, 300, NULL, NULL);
	singlePlayerPage->addMenuItem(backSinglePlayerItem);

	singlePlayerItem->setNextMenuPage(singlePlayerPage);
	backSinglePlayerItem->setNextMenuPage(parent);

	// Multi Player Menu
	settingsPage = new MenuPage();

	MenuItem *changeVideoSettingsItem =
	    new MenuItem("VIDEO", 400, 600, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeVideoSettingsItem);

	MenuItem *changeSoundSettingsItem =
	    new MenuItem("SOUND", 400, 500, NULL, NULL,
			 false, 1000, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeSoundSettingsItem);

	MenuItem *changeKbdSettingsItem =
	    new MenuItem("KEYBOARD", 400, 400, NULL, NULL,
			 false, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeKbdSettingsItem);

	MenuItem *changeMouseSettingsItem =
	    new MenuItem("MOUSE", 400, 300, NULL, NULL,
			 false, 0, 500, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(changeMouseSettingsItem);

	MenuItem *backMultiPlayerItem =
	    new MenuItem("BACK", 400, 200, NULL, NULL, true, 400,
			 0, ANIMATION_STRAIGHT);
	settingsPage->addMenuItem(backMultiPlayerItem);

	settingsItem->setNextMenuPage(settingsPage);
	backMultiPlayerItem->setNextMenuPage(parent);


	// Change video res
	videoResPage = new MenuPage();

	MenuItem *change_res_640_x_480 =
	    new MenuItem("640 X 480", 400, 600, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_640_x_480);
	MenuItem *change_res_800_x_600 =
	    new MenuItem("800 X 600", 400, 500, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_800_x_600);
	MenuItem *change_res_1024_x_768 =
	    new MenuItem("1024 X 768", 400, 400, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_1024_x_768);
	MenuItem *change_res_1600_x_1200 =
	    new MenuItem("1600 X 1200", 400, 300, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(change_res_1600_x_1200);
	MenuItem *backVidResItem =
	    new MenuItem("BACK", 400, 200, NULL, NULL, true, 400,
			 0, ANIMATION_STRAIGHT);
	videoResPage->addMenuItem(backVidResItem);

	changeVideoSettingsItem->setNextMenuPage(videoResPage);
	backVidResItem->setNextMenuPage(settingsPage);

	// Options Menu

	// Credits Menu
	creditsPage = new MenuPage(3);

	// Disabled Names
	MenuItem *bhanuItem =
	    new MenuItem("BHANU KALYAN", 400, 600, NULL, NULL, false);
	creditsPage->addMenuItem(bhanuItem);

	MenuItem *vamsiItem =
	    new MenuItem("VAMSI KRISHNA", 400, 500, NULL, NULL, false);
	creditsPage->addMenuItem(vamsiItem);

	MenuItem *swamyItem =
	    new MenuItem("SWAMY SUMAN", 400, 400, NULL, NULL, false);
	creditsPage->addMenuItem(swamyItem);

	MenuItem *backCreditsItem = new MenuItem("BACK", 400, 300, NULL, NULL);
	creditsPage->addMenuItem(backCreditsItem);

	creditsItem->setNextMenuPage(creditsPage);
	backCreditsItem->setNextMenuPage(parent);

	InitMenuOpenGL(1024, 768);

	InitFont();

	currentMenuPage->Show();
	currentMenuPage->Animate();

	initialized = true;
}

/* Called from timer func - so sync with process events and render */
void Menu::DestroyMenu(void)
{
	if (!initialized)
		return;

	initialized = false;

	/* OK now to make sure everyone has exit their blocks */
	lazy_destroy = true;
}

void Menu::LazyDestroyMenu(void)
{
	if (!lazy_destroy)
		return;

	assert(!initialized);

	printf("Lazy destroying menu!\n");

	DestroyFont();

	currentMenuPage = NULL;

	delete videoResPage; videoResPage = NULL;

	delete singlePlayerPage; singlePlayerPage = NULL;
	delete settingsPage; settingsPage = NULL;
	delete creditsPage; creditsPage = NULL;

	delete parent; parent = NULL;

	lazy_destroy = false;
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

	/* Check if we have any lazy destroy pending */
	if (lazy_destroy)
		LazyDestroyMenu();
}

void Menu::Render(void)
{
	if (!initialized)
		return;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static int linex1 = rand() * 1024;
	glColor3f( 1, 0, 0 );
	glLineWidth(5);
	glBegin(GL_LINES);
		glVertex3f(linex1++ , 0, -1.5);
		glVertex3f(linex1++ , 768, -1.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(linex * 3, 0, -1.5);
		glVertex3f(linex * 3, 768, -1.5);
	glEnd();

	glBegin(GL_LINES);
		glVertex3f(0, 200 + linex * 2, -1.5);
		glVertex3f(1024, 200 + linex * 2, -1.5);
	glEnd();
	glBegin(GL_LINES);
		glVertex3f(0, linex * 3, -1.5);
		glVertex3f(1024, linex * 3, -1.5);
	glEnd();

	glColor3f( 1, 1, 1 );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 52);
	glBegin(GL_QUADS);
		glTexCoord2f(0 ,0);
		glVertex3f(0, 0, -1.6);
		glTexCoord2f(0 ,1);
		glVertex3f(0, 768, -1.6);
		glTexCoord2f(1 ,1);
		glVertex3f(1024, 768, -1.6);
		glTexCoord2f(1 ,0);
		glVertex3f(1024, 0, -1.6);
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

