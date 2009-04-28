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
	if (currentSelection != currentMenuIndex) {
		currentSelection = currentMenuIndex;
		soundSystem->Play(SOUNDTYPE_MENU_TING);
	}
}

void MenuPage::addMenuItem(MenuItem * mi)
{
	menuItemArray[noOfMenuItems] = mi;
	noOfMenuItems++;
}

// Keyboard Functions
void MenuPage::moveUp(void)
{
	int sel = currentSelection;
 NEXT:	sel--;
	if (sel < 0)
		sel = noOfMenuItems - 1;

	if (!menuItemArray[sel]->isEnabled())
		goto NEXT;

	setCurrentMenuItem(sel);
}

void MenuPage::moveDown(void)
{
	int sel = currentSelection;
 NEXT:	sel++;
	if (sel >= noOfMenuItems)
		sel = 0;

	if (!menuItemArray[sel]->isEnabled())
		goto NEXT;

	setCurrentMenuItem(sel);
}

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
}

void MenuPage::mouseMove(int mousex, int mousey)
{
	for (int i = 0; i < noOfMenuItems; i++) {
		if (menuItemArray[i]->isEnabled()
		    && menuItemArray[i]->isInside(mousex, mousey)) {
				setCurrentMenuItem(i);
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
