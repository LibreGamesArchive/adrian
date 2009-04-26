#include "menupage.h"
#include "menu.h"
#include "../globals.h"

MenuPage::MenuPage(int currentSelection)
{
	noOfMenuItems = 0;
	animatePercent = 100;
	this->currentSelection = currentSelection;
	memset(menuItemArray, 0, sizeof(menuItemArray));
	parentMenuPage = NULL;
}

MenuPage::~MenuPage()
{
	for (int i = 0; i < noOfMenuItems; i++)
		delete menuItemArray[i];
}

// Accesor Functions
MenuPage *MenuPage::getParentPage(void)
{
	return parentMenuPage;
}

void MenuPage::setParentPage(MenuPage * parentMenuPage)
{
	this->parentMenuPage = parentMenuPage;
}

MenuItem *MenuPage::getCurrentMenuItem(void)
{
	return menuItemArray[currentSelection];
}

void MenuPage::setCurrentMenuItem(int currentMenuIndex)
{
	currentSelection = currentMenuIndex;
}

void MenuPage::addMenuItem(MenuItem * mi)
{
	menuItemArray[noOfMenuItems] = mi;
	noOfMenuItems++;
}

// Keyboard Functions
void MenuPage::moveUp(void)
{
 NEXT:	currentSelection--;
	if (currentSelection < 0)
		currentSelection = noOfMenuItems - 1;

	if (!menuItemArray[currentSelection]->isEnabled())
		goto NEXT;
}

void MenuPage::moveDown(void)
{
 NEXT:	currentSelection++;
	if (currentSelection >= noOfMenuItems)
		currentSelection = 0;

	if (!menuItemArray[currentSelection]->isEnabled())
		goto NEXT;

}

//extern void PlaySound(void);

void MenuPage::accept(void)
{
	if (((MenuPage *) menuItemArray[currentSelection]->getNextMenuPage())) {
		((MenuPage *) menuItemArray[currentSelection]->
		 getNextMenuPage())->parentMenuPage = this;
		menu->setCurrentMenuPage((MenuPage *) menuItemArray[currentSelection]->
		    getNextMenuPage());
	}
	if (menuItemArray[currentSelection]->getFunc()) {
		menuItemArray[currentSelection]->ExecuteFunc();
	}
//	PlaySound();
}

void MenuPage::mouseAccept(int mousex, int mousey)
{
	bool insideFlag = false;
	for (int i = 0; i < noOfMenuItems; i++) {
		if ((menuItemArray[i]->isEnabled()
		     && menuItemArray[i]->isInside(mousex, mousey))) {
			insideFlag = true;
			break;
		}
	}

	if (!insideFlag)
		return;

	if (menuItemArray[currentSelection]->getFunc()) {
		menuItemArray[currentSelection]->ExecuteFunc();
	}
	if (((MenuPage *) menuItemArray[currentSelection]->getNextMenuPage())) {
		((MenuPage *) menuItemArray[currentSelection]->
		 getNextMenuPage())->parentMenuPage = this;
		menu->setCurrentMenuPage((MenuPage *) menuItemArray[currentSelection]->
		    getNextMenuPage());
	}
//	PlaySound();
}

void MenuPage::mouseMove(int mousex, int mousey)
{
	for (int i = 0; i < noOfMenuItems; i++) {
		if (menuItemArray[i]->isEnabled()
		    && menuItemArray[i]->isInside(mousex, mousey)) {
			currentSelection = i;
		}
	}
}

void MenuPage::Render(void)
{
	for (int i = 0; i < noOfMenuItems; i++) {
		if (i == currentSelection) {
			menuItemArray[i]->Render(MENUITEM_HIGHLIGHT);
		} else {
			menuItemArray[i]->Render(MENUITEM_ORDINARY);
		}
	}
}

void MenuPage::Dump(void)
{
	for (int i = 0; i < noOfMenuItems; i++)
		menuItemArray[i]->Dump();
}

void MenuPage::Show(void)
{
	animatePercent = 0;
	for (int j = 0; j < noOfMenuItems; j++) {
		menuItemArray[j]->Show();
	}

}

void MenuPage::Hide(void)
{
	animatePercent = 0;
	for (int j = 0; j < noOfMenuItems; j++) {
		menuItemArray[j]->Hide();
	}

}

void MenuPage::Animate(void)
{
	if (animatePercent >= 100) {
		animatePercent = 100;
		return;
	}
	animatePercent += 1;
	for (int j = 0; j < noOfMenuItems; j++) {
		menuItemArray[j]->Animate(animatePercent);
	}
}

void MenuPage::toPreviousMenuPage(void)
{
	if( parentMenuPage != NULL )
	{
//		((MenuPage *)menuItemArray[noOfMenuItems]->getNextMenuPage())->parentMenuPage = this;
//		currentMenuPage = parentMenuPage;
//		currentMenuPage->Show();
	}
	else
	{
		exit(0);
	}
}

bool MenuPage::isAnimating(void)
{
	if (animatePercent >= 100)
		return false;
	return true;
}
