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
	SDL_Renderer *ren;
	SDL_Texture *icon;	//Image of this module tile
	SDL_Rect *srcRect;	//Take this rectangle...
	SDL_Rect *dstRect;	//...and place it here!
	int nameId;			//Refrence to string arrayslot (server)
	int maxHealth;		//Maximum health level
	int currentHealth;	//Current health level
	int currentPower;	//Current power stored
	int disabled;		//Turns disabled
	int accuracy;		//Chance to hit

	int activeTurns;	//Turns this stays active
	int activeLeft;		//How many of these are left
	bool active;

	bool hovered;
	bool held;

	int requiredPower;	//Power required to use
	int changedPower;	//Stores the change in power to this module
	int damage;			//Damage this gun does
	int defence;		//The damage this absorbs
	int disablePower;	//Turns this gun may disable on hit

	int activeLostOnHit; //How many turns shield looses for being hit.

	Text *healthText;
	SDL_Texture *iconPower;
	SDL_Texture *iconPowerEmpty;
	SDL_Texture *iconHighlight;
	SDL_Texture *iconActivate; //Icon of bullet or shield
	SDL_Rect *srcEffect;
	SDL_Rect *srcRocket;
	SDL_Rect *dstRocket;
	int rocketSpeed;
	bool mirrorRocket;
	bool runningAnimation;

	int targetX;
	int targetY;
	int targetPosX;
	int targetPosY;

public:
	Module();
	Module(SDL_Renderer *rend, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int hullId = -1, int reqPow = 0, int dmg = 0, int disPow = 0, int act = 0);
	~Module();

	
	void printData();					//Shows the data listed in console
	int getMaxHealth();					//Returns max health
	int getCurrentHealth();				//Returns current health
	void onHit(int dmg);				//When THIS module is hit
	void draw(bool computer);			//Draw the module
	SDL_Rect getSource();				//Gets the source rectangle
	SDL_Rect getDestination();			//Gets the destination rectangle
	module_action onMouseClick(SDL_Event event);	//Clicked on the texture
	bool isMouseOver(SDL_Event event);	//Checks if mouse is in bounderies
	void setPosition(int posX, int posy);
	bool addEnergy();
	bool removeEnergy();
	void resetEnergy();
	int getCurrentEnergy();
	int getReqPower();
	bool canActivate();
	bool getActive();
	void setHeld(bool state);
	bool getHeld();
	void clearTarget();
	void setTarget(int x, int y, int posX, int posY);
	void getTarget(int &x, int &y, int &dmg);
	void getPosition(int &posX, int &posY);
	bool isDead();
	void restore();
	void setTargetLineToMouse(int mouseX = -1, int mouseY = -1);
	bool hasTarget();
	void drawInterface();
	bool isShielding();
	void damageShield();
	bool runRocketAnimation(Module *end);
	string registerAttack(int x, int y);
	int getType();
	void setActive();
	int getChangedEnergy();
	int getDamage();
	int getDefence();
	int getActiveLeft();
	int getActiveLostOnHit();
};