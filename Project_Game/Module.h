#pragma once
#include <SDL.h>
#include <string>
#include <iostream>
using namespace std;



class Module
{
private:
	SDL_Surface *icon;	//Image of this module tile
	int nameId;			//Refrence to string arrayslot
	int maxHealth;		//Maximum health level
	int currentHealth;	//Current health level
	int currentPower;	//Current power stored
	int disabled;		//Turns disabled
	int accuracy;		//Chance to hit

public:
	Module();
	Module(string ico, int namId, int maxHp, int acc);
	~Module();

	void printData();		//Shows the data listed in console
	int getMaxHealth();		//Returns max health
	int getCurrentHealth();	//Returns current health
	void onHit(int dmg);	//When THIS module is hit
};