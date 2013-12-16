#pragma once
#include "Space_Ship.h"
#include "Module.h"
#include "Hull.h"

class Ai
{
private:
	Space_Ship *aiShip;
	Space_Ship *playerShip;
	int defPri;
	int attackPri;

public:
	Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_pri, int attack_Pri);
	int placeEnergy();
	int getAttack();
	~Ai();
};

