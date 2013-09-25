#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;



class Module
{
private:
    SDL_Surface *iconIMG; //
	
	int nameId;			//Refrence to string arrayslot
	int maxHealth;		//Maximum health level
	int currentHealth;	//Current health level
	int currentPower;	//Current power stored
	int disabled;		//Turns disabled
	int accuracy;		//Chance to hit
    SDL_Rect postion;   //Postion of the icon

public:
	Module();
	Module(SDL_Renderer *ren, string ico, int namId, int maxHp, int acc);
	~Module();
SDL_Texture *icon;	//Image of this module tile
	void printData();				//Shows the data listed in console
	int getMaxHealth();				//Returns max health
	int getCurrentHealth();			//Returns current health
	void onHit(int dmg);			//When THIS module is hit
	void draw(SDL_Renderer *ren);	//Draw the module
    void setPostion(int x, int y);
    void setSize(int w, int h);
    SDL_Rect getPostion();
};