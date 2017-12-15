#pragma once
#include "Message.h"
#include "MessagingSystem.h"
#include <memory>

class MenuEngine
{
	public:
		MenuEngine(int x, int y, int width, int height);
		~MenuEngine();
		bool checkBounds(int posX, int posY);
		void initializeMenuItems();
		void menuItemInteraction(MenuEngine B1, MenuEngine B2, MenuEngine B3);

	private:
		int x;
		int y;
		int width;
		int height;
};