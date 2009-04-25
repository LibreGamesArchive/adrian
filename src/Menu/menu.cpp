#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>

#include "menu.h"
#include "../tga.h"

extern float FontTexture[256][2];

void InitFont(void)
{
	if (loadTGA("font.tga", 20) != 0) {
		printf("Could not load textures\n");
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

void exitGame(void)
{
//      SDL_Quit();
	exit(0);
}

void InitializeMenu(void)
{
	// Main Menu
	MenuPage *parent = new MenuPage();

	MenuItem *singlePlayerItem =
	    new MenuItem("SINGLE PLAYER", 400, 600, NULL, NULL, true,
			 0, 600);
	parent->addMenuItem(singlePlayerItem);

	MenuItem *multiPlayerItem =
	    new MenuItem("MULTI PLAYER", 400, 500, NULL, NULL, true, 900, 500);
	parent->addMenuItem(multiPlayerItem);

	// Disabled
	MenuItem *optionsItem =
	    new MenuItem("OPTIONS", 400, 400, NULL, NULL, false, 0, 0);
	parent->addMenuItem(optionsItem);

	MenuItem *creditsItem =
	    new MenuItem("CREDITS", 400, 300, NULL, NULL, true, 400, 0);
	parent->addMenuItem(creditsItem);

	MenuItem *quitItem =
	    new MenuItem("QUIT", 400, 200, (void *)exitGame, NULL, true, 400,
			 700);
	parent->addMenuItem(quitItem);

	currentMenuPage = parent;

	// Single Player Menu
	MenuPage *singlePlayerPage = new MenuPage();

	MenuItem *startSinglePlayerGameItem =
	    new MenuItem("START GAME", 400, 500, NULL, NULL);
	singlePlayerPage->addMenuItem(startSinglePlayerGameItem);

	MenuItem *loadSinglePlayerGameItem =
	    new MenuItem("LOAD GAME", 400, 400, NULL, NULL);
	singlePlayerPage->addMenuItem(loadSinglePlayerGameItem);

	MenuItem *backSinglePlayerItem =
	    new MenuItem("BACK", 400, 300, NULL, NULL);
	singlePlayerPage->addMenuItem(backSinglePlayerItem);

	singlePlayerItem->setNextMenuPage(singlePlayerPage);
	backSinglePlayerItem->setNextMenuPage(parent);

	// Multi Player Menu
	MenuPage *multiPlayerPage = new MenuPage();

	MenuItem *startMultiPlayerServerItem =
	    new MenuItem("START SERVER", 400, 500, NULL, NULL,
			 true, 0, 500, ANIMATION_STRAIGHT);
	multiPlayerPage->addMenuItem(startMultiPlayerServerItem);

	MenuItem *joinMultiPlayerServerItem =
	    new MenuItem("JOIN SERVER", 400, 400, NULL, NULL,
			 true, 1000, 500, ANIMATION_STRAIGHT);
	multiPlayerPage->addMenuItem(joinMultiPlayerServerItem);

	MenuItem *backMultiPlayerItem =
	    new MenuItem("BACK", 400, 300, NULL, NULL, true, 400,
			 0, ANIMATION_STRAIGHT);
	multiPlayerPage->addMenuItem(backMultiPlayerItem);

	multiPlayerItem->setNextMenuPage(multiPlayerPage);
	backMultiPlayerItem->setNextMenuPage(parent);

	// Options Menu

	// Credits Menu
	MenuPage *creditsPage = new MenuPage(3);

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

	parent->Dump();

	InitFont();
}
