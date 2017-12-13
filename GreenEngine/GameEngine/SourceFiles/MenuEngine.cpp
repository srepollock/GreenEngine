#include "MenuEngine.h"

MenuEngine::MenuEngine(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;

	this->width = width;
	this->height = height;
}

MenuEngine::~MenuEngine()
{
	
}

bool MenuEngine::checkBounds(int posX, int posY)
{
	if (posX >= x && posX <= x + width)
	{
		if (posY >= y && posY <= y + height)
		{
			return true;
		}
	}
	return false;
}

void MenuEngine::initializeMenuItems()
{
	MenuEngine play(0, 0, 20, 20);
	MenuEngine settings(0, 25, 20, 20);
	MenuEngine exit(0, 50, 20, 20);
	menuItemInteraction(play, settings, exit);
}

void MenuEngine::menuItemInteraction(MenuEngine playButton, MenuEngine settingsButton, MenuEngine exitButton) 
{
	/*int mouseX, mouseY;

	if (playButton.checkBounds(mouseX, mouseY))
	{

	}

	if (settingsButton.checkBounds(mouseX, mouseY))
	{

	}

	if (exitButton.checkBounds(mouseX, mouseY))
	{

	}*/
}