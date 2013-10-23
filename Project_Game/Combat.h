#pragma once
#include "Space_Ship.h"

class Combat
{
private:
	Space_Ship you;
	Space_Ship enemy;
	bool yourTurn;

public:
	Combat();
	Combat(Space_Ship yourShip, Space_Ship enemyShip, bool yourTurn);
	void makeMoves();
	void listenForMoves();
};