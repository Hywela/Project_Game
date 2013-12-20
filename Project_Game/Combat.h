#pragma once
#include "Space_Ship.h"
#include <windows.h>
#include "Button.h"
#include "Ai.h"
#include <vector>
#include <sstream>
#include "Network.h"
using namespace std;



class Combat
{
private:
	Network *server;
	Space_Ship *you;
	Space_Ship *enemy;
	Ai *ai;
	bool yourTurn;
	SDL_Event event;
	SDL_Renderer *ren;
	SDL_Texture *background;
	SDL_Window *win;
	vector <Button*> buttons;
	vector <string> yourAction;
	vector <string> enemyAction;
	bool doneAnimating;
	Text *statusEnergyLeft;
	bool surrender;

public:
	Combat();
	Combat(Space_Ship *yourShip, Space_Ship *enemyShip, bool turnStart, SDL_Renderer *rend, SDL_Window *wind, Network *instanceOfServer = NULL);
	void setupInterface();
	void makeMovesPVP();
	void makeMoves();
	void listenForMovesPVP();
	void listenForMoves();
	void draw();
	void playAnimation(string attackCode);
	void setupAttacksPVP();
	void setupAttacks();
	void prepareShip();
	
};