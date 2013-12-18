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
	int modules[SHIP_HEIGHT*SHIP_WIDTH];
	for (int i = 0; i < (SHIP_HEIGHT*SHIP_WIDTH); i++){ //reset pri array to 0
		modules[i] = 0;
	}

	//Look through modules 
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			Module *modu = aiShip->getModule(row, collumn);
			if (modu != NULL){	//if there is a module at position
				if (modu->getCurrentHealth() > 0){	//if alive
					if (modu->getCurrentEnergy() < modu->getReqPower()){//if not full of energy
						cout << moduleIndex << " Current energy " << modu->getCurrentEnergy() << endl;
						modules[moduleIndex] += SHIP_HEIGHT*SHIP_WIDTH;
					}
				}
			}
			moduleIndex++;
		}
	}

	for (int i = 0; i < (SHIP_HEIGHT*SHIP_WIDTH); i++){
		cout << i << " Is prioritized at " << modules[i] << endl;
		
	}

	int topPri = -1;
	for (int i = 0; i < SHIP_HEIGHT*SHIP_WIDTH; i++){
		if (modules[i] > 0 && modules[i] >topPri){
			topPri = i;
		}
	}

	cout << "Top pri: " << topPri << endl ;
	//system("pause");
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

void Ai::aiActions(){
	
	//load in ship information
	for (int row = 0; row < SHIP_HEIGHT; row++){
		for (int collumn = 0; collumn < SHIP_WIDTH; collumn++){
			if (playerShip->getModule(row, collumn) != NULL){
				moduleHp[collumn][row] = playerShip->getModule(row, collumn)->getCurrentHealth();
			}
			if (aiShip->getModule(row, collumn) != NULL){
				modulePow[collumn][row] = aiShip->getModule(row, collumn)->getCurrentEnergy();
			}
		}
	}

	/*
	
	

	//Placing energy
	for (int i = 0; i < aiShip->getMaxEnergy(); i++){
		int modNr = placeEnergy();
		if (modNr >= 0){
			cout << modNr << "Placing energy: " << modNr % SHIP_WIDTH << " " << modNr / SHIP_HEIGHT << endl;
			aiShip->getModule((modNr / SHIP_HEIGHT), (modNr % SHIP_WIDTH))->addEnergy();
		}
		else{
			cout << "NO modules to power up\n";
		}
	}

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
