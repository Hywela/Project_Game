#include "Ai.h"


Ai::Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_Pri, int attack_Pri)
{
	aiShip = ai_Ship;
	playerShip = player_Ship;
	defPri = def_Pri+SHIP_HEIGHT*SHIP_WIDTH;
	attackPri = attack_Pri + SHIP_HEIGHT*SHIP_WIDTH;

}

int Ai::energyTarget(){
	int moduleIndex = 0;
	int modules[SHIP_HEIGHT*SHIP_WIDTH];
	for(int i = 0; i < (SHIP_HEIGHT*SHIP_WIDTH); i++){ //reset pri array to 0
		modules[i] = 0;
	}

	//Look through modules 
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *modu = aiShip->getModule(row, collumn);
			if (modu != NULL){	//if there is a module at position
				if (modu->getCurrentHealth() > 0){	//if alive
					if (modulePow[row][collumn] < modu->getReqPower()){//if not full of energy
						cout << moduleIndex << " Current energy " << modulePow[collumn][row] << endl;
						modules[moduleIndex] += SHIP_HEIGHT*SHIP_WIDTH;
					}
				}
			}
			moduleIndex++;
		}
	}

	

	int topPri = -1;
	for (int i = 0; i < SHIP_HEIGHT*SHIP_WIDTH; i++){
		cout << i << " Is prioritized at " << modules[i] << endl;
		if (modules[i] > 0 && modules[i] >topPri){
			topPri = i;
		}
	}

	cout << "Top pri: " << topPri << endl ;
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
				if (moduleHp[collumn][row] > 0){
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

void Ai::aiActions(){
	
	//load in ship information
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			if (playerShip->getModule(row, collumn) != NULL){
				moduleHp[collumn][row] = playerShip->getModule(row, collumn)->getCurrentHealth();
			}else{
				moduleHp[collumn][row] = 0;
			}
			if (aiShip->getModule(row, collumn) != NULL){
				modulePow[collumn][row] = aiShip->getModule(row, collumn)->getCurrentEnergy();
			}else{
				modulePow[collumn][row] = 0;
			}

		}
	}

	//Place energy
	for (int i = 0; i < aiShip->getMaxEnergy(); i++){
		int modNr = energyTarget();
		if (modNr >= 0){
			cout << modNr << " Placing energy: " << modNr % SHIP_WIDTH << " " << modNr / SHIP_HEIGHT << endl;
			aiShip->getModule((modNr / SHIP_HEIGHT), (modNr % SHIP_WIDTH))->addEnergy();		//add energy
			modulePow[(modNr / SHIP_HEIGHT)][(modNr % SHIP_WIDTH)]++;
		}
		else{
			cout << "NO modules to power up\n";
		}
	}

	//for each attack module with full energy
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *atkModule = aiShip->getModule(row, collumn);
			//if an allive attack module with full power
			if (atkModule != NULL && modulePow[collumn][row] == atkModule->getReqPower() && atkModule->getCurrentHealth() > 0 && atkModule->getType() == TURRET){
				int modNr = getAttack();

				//Attack enemy module
				Module *target = playerShip->getModule((modNr / SHIP_HEIGHT), (modNr % SHIP_WIDTH));
				int pixCoorX, pixCoorY; //pixel coordinates of target module...
				target->getPosition(pixCoorX, pixCoorY);	//get coordinates from target module
				atkModule->setTarget(row, collumn, pixCoorX, pixCoorY); //set attack modules target
				moduleHp[collumn][row] -= atkModule->getDamage();		
			}
		}
	}

	/*
	//Attacking with all powered gunns
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *modu = aiShip->getModule(row, collumn);
			if (modu != NULL && modu->getCurrentEnergy() == modu->getReqPower() && modu->getCurrentHealth() > 0){
				int modNr = getAttack();
				testShip.attack((modNr % SHIP_WIDTH), (modNr / SHIP_HEIGHT), modu->getDamage());
			}
		}
	}

	*/

}


Ai::~Ai()
{
}
