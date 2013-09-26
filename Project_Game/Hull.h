#pragma once
#include <string>
#include <SDL.h>
using namespace std;



class Hull
{
private:
	SDL_Texture *icon;	//The image of this chassis tile.
	int armor;			//The armor value of the chassis piece.
	int specialAttack;	//Stat to boost attack units in this tile.
	int specialDefence;	//Stat to boost defencive units in this tile.

public:
    Hull();
	Hull(string ico, int arm, int spAtk, int spDef);
    ~Hull();

	void draw(SDL_Renderer *ren);
};