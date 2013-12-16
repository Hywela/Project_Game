#include "Ai.h"


Ai::Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_Pri, int attack_Pri)
{
	aiShip = ai_Ship;
	playerShip = player_Ship;
	defPri = def_Pri+SHIP_HEIGHT*SHIP_WIDTH;
	attackPri = attack_Pri + SHIP_HEIGHT*SHIP_WIDTH;

}

int Ai::placeEnergy(){
	int moduleIndex = 0;
	int modules[SHIP_HEIGHT*SHIP_WIDTH] = { 0 };
	//Look through modules 
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *modu = aiShip->getModule(row, collumn);
			if (modu != NULL){
				if (modu->getCurrentHealth() > 0 && modu->getCurrentEnergy() < modu->getReqPower()){
					modules[moduleIndex] += SHIP_HEIGHT*SHIP_WIDTH;
				}
			}
			moduleIndex++;
		}
	}

	int topPri = -1;
	for (int i = 0; i < SHIP_HEIGHT*SHIP_WIDTH; i++){
		if (modules[i] > 0 && modules[i] >topPri){
			topPri = i;
		}
	}

	return topPri;
}

int Ai::getAttack(){
	int moduleIndex = 0;
	int modules[SHIP_HEIGHT*SHIP_WIDTH] = { 0 };
	//Look through modules 
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *modu = playerShip->getModule(row, collumn);
			if (modu != NULL){
				if (modu->getCurrentHealth() > 0){
					modules[moduleIndex] += SHIP_HEIGHT*SHIP_WIDTH;
				}
			}
			moduleIndex++;
		}
	}

	int topPri = -1;
	for (int i = 0; i < SHIP_HEIGHT*SHIP_WIDTH; i++){
		if (modules[i] > 0 && modules[i] >topPri){
			topPri = i;
		}
	}

	return topPri;
}


Ai::~Ai()
{
}
