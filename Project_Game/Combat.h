#pragma once
#include "Space_Ship.h"
#include <windows.h>
#include "Button.h"



class Combat
{
private:
	Space_Ship *you;
	Space_Ship *enemy;
	bool yourTurn;
	SDL_Event event;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Window *win;
	vector <Button*> buttons;
	vector <string> yourAction;
	vector <string> enemyAction;
	int energyLeft;

public:
	Combat();
	Combat(Space_Ship *yourShip, Space_Ship *enemyShip, bool youStart, SDL_Renderer *rend, SDL_Window *wind);
	void makeMoves();
	void listenForMoves();
};