#pragma once
#include "display.h"
#include <SDL.h>
#include <string>
using namespace std;



class Hull : private Display
{
private:
	SDL_Surface *icon;	//The image of this chassis tile.
	int armor;			//The armor value of the chassis piece.
	int specialAttack;	//Stat to boost attack units in this tile.
	int specialDefence;	//Stat to boost defencive units in this tile.

public:
    Hull();
	Hull(string ico, int arm, int spAtk, int spDef);
    ~Hull();
};