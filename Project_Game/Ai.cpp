#include "Ai.h"


Ai::Ai(Space_Ship *ai_Ship, Space_Ship *player_Ship, int def_Pri, int attack_Pri)
{
	aiShip = ai_Ship;
	playerShip = player_Ship;
	defPri = def_Pri+(SHIP_HEIGHT*SHIP_WIDTH);
	attackPri = attack_Pri + (SHIP_HEIGHT*SHIP_WIDTH);

}

int Ai::energyTarget(){
	static int priToFillUpModules = 3;
	int moduleIndex = 0;
	int modules[SHIP_HEIGHT*SHIP_WIDTH];
	for(int i = 0; i < (SHIP_HEIGHT*SHIP_WIDTH); i++){ //reset pri array to 0
		modules[i] = 0;
	}

	//Look through modules 
	for (int y = 0; y < SHIP_HEIGHT; y++){
		for (int x = 0; x < SHIP_WIDTH; x++){
			Module *modu = aiShip->getModule(y, x);
			if (modu != NULL){	//if there is a module at position
				if (modu->getCurrentHealth() > 0){	//if alive
					if (modulePow[y][x] < modu->getReqPower() && !modu->getActive()){//if not full of energy
						cout << "\n\nModule: " << moduleIndex;
						modules[moduleIndex] += SHIP_HEIGHT*SHIP_WIDTH;
						if (modu->getType() == TURRET){			//check if turret
							cout << " TURRET add \nattackPri \t\t + " << attackPri << endl;
							modules[moduleIndex] += attackPri;
						}
						else{	//check if shield
							cout << " SHIELD add \ndefPri \t\t\t + " << defPri << endl << "LostOnHit \t\t - " << modu->getActiveLostOnHit() << endl;
							modules[moduleIndex] += defPri;
							modules[moduleIndex] = modules[moduleIndex] - modu->getActiveLostOnHit();
						}
						if (energyLeft + modu->getCurrentEnergy() < modu->getReqPower()){	//if not enough energy to fill a module
							if (aiShip->isShielded(x, y) > 0){		//add priority for shielded modules
								modules[moduleIndex] += (aiShip->isShielded(x, y) / 10); // shielded in % /10
								cout << "isShielded \t\t + " << aiShip->isShielded(x, y) / 10 << endl;
							}
							modules[moduleIndex] += modu->getCurrentHealth();			//prefer a module with the most health left
							cout << "CurrentHp \t\t + " << modu->getCurrentHealth() << endl;
						}
						else{ //if possibility to fill up a module prefer the modules that have the highest effect
							if (modu->getType() == TURRET){			//check if turret
								modules[moduleIndex] += (modu->getDamage() * priToFillUpModules);
								cout << "Damage \t\t\t + " << modu->getDamage() << endl;
							}
							else{	//check if shield
								modules[moduleIndex] += getSuroundingModulHp(aiShip, x, y);
								cout << "SurroundingHP \t\t + " << getSuroundingModulHp(aiShip, x, y) << endl;
							}
						}
						if (modu->getCurrentEnergy() > 0){		//if module is partly filled up already
							modules[moduleIndex] += (modu->getCurrentEnergy() * priToFillUpModules);
							cout << "CurEnergy \t\t + " << modu->getCurrentEnergy() << endl;
						}
						cout << "TOTAL::: \t\t = " << modules[moduleIndex] << endl<<endl;
					}
				}
			}
			moduleIndex++;
		}
	}

	


	int topPri = -1;
	int topPriModule = -1;
	for (int i = 0; i < SHIP_HEIGHT*SHIP_WIDTH; i++){
		if (modules[i] > 0 && modules[i] > topPri){
			topPri = modules[i];
			topPriModule = i;
		}
	}

	cout << "Top Pri: " << topPriModule << endl;
	return topPriModule;
}

int Ai::getAttack(){
	int moduleIndex = 0;
	int modules[SHIP_HEIGHT*SHIP_WIDTH] = { 0 };
	//Look through modules 
	for (int y = 0; y < SHIP_HEIGHT; y++){
		for (int x = 0; x < SHIP_WIDTH; x++){
			Module *modu = playerShip->getModule(y, x);
			if (modu != NULL){
				if (moduleHp[y][x] > 0){
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
	energyLeft = aiShip->getMaxEnergy();

	for (int y = 0; y < SHIP_HEIGHT; y++){
		for (int x = 0; x < SHIP_WIDTH; x++){
			if (playerShip->getModule(y, x) != NULL){
				moduleHp[y][x] = playerShip->getModule(y, x)->getCurrentHealth();
			}else{
				moduleHp[y][x] = 0;
			}
			if (aiShip->getModule(y, x) != NULL){
				modulePow[y][x] = aiShip->getModule(y, x)->getCurrentEnergy();
				//cout << "Module y:" << y << " x:" << x << " got Energy: " << modulePow[y][x] << endl;
			}else{
				modulePow[y][x] = 0;
			}

		}
	}

	//Place energy
	for (int i = 0; i < aiShip->getMaxEnergy(); i++){
		int modNr = energyTarget();
		if (modNr >= 0){
			//cout << modNr << " Placing energy: " << modNr % SHIP_WIDTH << " " << modNr / SHIP_HEIGHT << endl;
			aiShip->getModule((modNr / SHIP_HEIGHT), (modNr % SHIP_WIDTH))->addEnergy();		//add energy
			modulePow[(modNr / SHIP_HEIGHT)][(modNr % SHIP_WIDTH)]++;
			energyLeft--;
		}
		else{
			cout << "NO modules to power up\n";
		}
	}

	//for each attack module with full energy
	for (int y = 0; y < SHIP_HEIGHT; y++){
		for (int x = 0; x < SHIP_WIDTH; x++){
			Module *atkModule = aiShip->getModule(y, x);
			//if an allive attack module with full power
			if (atkModule != NULL && modulePow[y][x] == atkModule->getReqPower() && atkModule->getCurrentHealth() > 0 && atkModule->getType() == TURRET){
				int modNr = getAttack();
				if (modNr >= 0){
					//Attack enemy module
					Module *target = playerShip->getModule((modNr / SHIP_HEIGHT), (modNr % SHIP_WIDTH));
					int pixCoorX, pixCoorY; //pixel coordinates of target module...
					target->getPosition(pixCoorX, pixCoorY);	//get coordinates from target module
					atkModule->setTarget((modNr % SHIP_WIDTH), (modNr / SHIP_HEIGHT), pixCoorX, pixCoorY); //set attack modules target
					moduleHp[(modNr / SHIP_HEIGHT)][(modNr % SHIP_WIDTH)] -= atkModule->getDamage();
					//cout << "Module x:" << x << " y:" << y << " got Energy: " << modulePow[y][x] << endl;
					//cout << modNr << " Placing Target: " << modNr % SHIP_WIDTH << " " << modNr / SHIP_HEIGHT << endl;
				}
			}
		}
	}
}


Ai::~Ai()
{
}


int Ai::getSuroundingModulHp(Space_Ship *ship, int modulX, int modulY)
{
	int surroundingHp = 0;
	//Check if module is shielded
	for (int y = modulY - 1; y <= modulY + 1; y++)
	{
		for (int x = modulX - 1; x <= modulX + 1; x++)
		{
			//If not out of bounderies
			if ((x >= 0 && x < SHIP_WIDTH) && (y >= 0 && y < SHIP_HEIGHT))
			{
				if (ship->getModule(y, x) != NULL)
				{
					surroundingHp += ship->getModule(y, x)->getCurrentHealth();
				}
			}
		}
	}
	return surroundingHp;
}