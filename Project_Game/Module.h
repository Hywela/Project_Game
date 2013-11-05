#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Constants.h"
#include "Text.h"
using namespace std;



class Module
{
private:
	SDL_Texture *icon;	//Image of this module tile
	SDL_Rect *srcRect;	//Take this rectangle...
	SDL_Rect *dstRect;	//...and place it here!
	int nameId;			//Refrence to string arrayslot
	int maxHealth;		//Maximum health level
	int currentHealth;	//Current health level
	int currentPower;	//Current power stored
	int disabled;		//Turns disabled
	int accuracy;		//Chance to hit

	int requiredPower;	//Power required to use
	int damage;			//Damage this gun does
	int disablePower;	//Turns this gun may disable on hit

	Text *healthText;
	SDL_Texture *iconPower;
	SDL_Texture *iconPowerEmpty;

public:
	Module();
	Module(SDL_Renderer *ren, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow = 0, int dmg = 0, int disPow = 0);
	~Module();

	void printData();					//Shows the data listed in console
	int getMaxHealth();					//Returns max health
	int getCurrentHealth();				//Returns current health
	void onHit(int dmg);				//When THIS module is hit
	void draw(SDL_Renderer *ren);		//Draw the module
	SDL_Rect getSource();				//Gets the source rectangle
	SDL_Rect getDestination();			//Gets the destination rectangle
	module_action onMouseClick(SDL_Event event);	//Clicked on the texture
	bool isMouseOver(SDL_Event event);	//Checks if mouse is in bounderies
	void setPosition(int posX, int posy);
	bool addEnergy();
	bool removeEnergy();
	void resetEnergy();
	bool activate();
};