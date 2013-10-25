#pragma once
#include "Space_Ship.h"
#include <windows.h>



class Combat
{
private:
	Space_Ship you;
	Space_Ship enemy;
	bool yourTurn;

public:
	Combat();
	Combat(Space_Ship yourShip, Space_Ship enemyShip, bool youStart);
	void makeMoves();
	void listenForMoves();
};