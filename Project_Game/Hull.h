#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Constants.h"
using namespace std;



class Hull
{
private:
	SDL_Renderer *ren;
	SDL_Texture *icon;	//Image of this hull tile
	SDL_Rect *srcRect;	//Take this rectangle...
	SDL_Rect *dstRect;	//...and place it here!
	int armor;			//The armor value of the chassis piece.
	int specialAttack;	//Stat to boost attack units in this tile.
	int specialDefence;	//Stat to boost defencive units in this tile.
	bool isLocked;		//If the tile is attached to something.
	bool isEmpty;		//If there is a tile here
	bool isPlaceable;	//If you can place a tile here
	int numAnchors;		//Number of sides connected

public:
    Hull();
	Hull(SDL_Renderer *rend, SDL_Rect src, SDL_Rect dst, string ico, int arm, int spAtk, int spDef, bool isLoc, bool isEmp, bool isPla);
    ~Hull();

	void drawBuild();	//Drawing during edit mode
	void draw();		//Drawing normally
	SDL_Rect getSource();				//Gets the source rectangle
	SDL_Rect getDestination();			//Gets the destination rectangle
	hull_action onMouseClick(SDL_Event event);	//Clicked on the texture
	bool isMouseOver(SDL_Event event);	//Checks if mouse is in bounderies
	void setLocked(bool state);			//Is this locked from changes
	void setEmpty(bool state);			//Is there a tile here
	void setPlaceable(bool state);		//Can you place a tile here
	int getAnchors();					//How many sides are connected
	void setAnchors(int num);			//Sets how many sides are connected
	bool isNotUsed();						//Checks if the tile is not used
	void setPosition(int x, int y);
};