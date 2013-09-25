#pragma once
#include <string>

#include "Module.h"
using namespace std;



class Module_Gun : public Module
{
private:
	int requiredPower;	//Power required to use
	int damage;			//Damage this gun does
	int disablePower;	//Turns this gun may disable on hit

public:
	Module_Gun();
	Module_Gun(SDL_Renderer *ren, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow, int dmg, int disPow);
	~Module_Gun();

	void printData();	//Shows the data listed in console
};