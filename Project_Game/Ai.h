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
	int defPri;												//Initial priority for defence
	int attackPri;											//Initial priority for offence
	int moduleHp[SHIP_HEIGHT][SHIP_WIDTH];					//save the changes that will be done after actions
	int modulePow[SHIP_HEIGHT][SHIP_WIDTH];		
	int energyLeft;											//energy the ai have left, to check if its enough to compleatly fill a module
	int enemyAtkModules;		//counts the number of enemy defencive modules
	int enemyDefModules;		//counts the number of enemy offencive modules
	int enemyUnspentEnergy;		//saves how much unused energy there is in the enemies modules

	int getSuroundingModulDamage(Space_Ship *ship, int modulX, int modulY);	//reurns the gathered damage potetial of modules around selected module, including selected
	int energyTarget();								//returns prioritized module to defend
	int getAttack(Module *attackingModule);			//returns prioritized module to attack

public:
	Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_pri, int attack_Pri);	
	void aiActions(); //start calculation ai actions and set energy and targets
	~Ai();
};

