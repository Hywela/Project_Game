#pragma once
#include "Space_Ship.h"
#include "Module.h"
#include "Hull.h"
#include <vector>

#define priToFillUpModules 3
#define priToPoweredModules 10
#define basePri 100

class Ai
{
private:
	Space_Ship *aiShip;
	Space_Ship *playerShip;
	int defPri;
	int attackPri;
	Space_Ship testShip;
	int moduleHp[SHIP_HEIGHT][SHIP_WIDTH];
	int modulePow[SHIP_HEIGHT][SHIP_WIDTH];
	int atkModules;
	int shieldModules;
	int energyLeft;
	int getSuroundingModulDamage(Space_Ship *ship, int modulX, int modulY);
	int enemyAtkModules;
	int enemyDefModules;
	int enemyUnspentEnergy;


public:
	Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_pri, int attack_Pri);
	int energyTarget();
	int getAttack(Module *attackingModule);
	void aiActions();
	~Ai();
};

