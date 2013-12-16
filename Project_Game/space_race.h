#pragma once
#include "Space_Ship.h"

#include "Button.h""
#include "Bullet.h"
#include <deque>
class space_race
{
private:
	Space_Ship *you;

	bool yourTurn;
	SDL_Event event;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Window *win;
	vector <Button*> buttons;
	vector <string> yourAction;
	vector <string> enemyAction;
	Text *statusEnergyLeft;
	int x, y;
	bool youAlive,shooting;
	deque<Bullet*> bullets;
public:
	space_race();
	space_race(Space_Ship *yourShip, bool youStart, SDL_Renderer *rend, SDL_Window *wind);
	void makeMoves();
	void listenForMoves();
	void draw();
	void shoot();

};

