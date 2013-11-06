#pragma once
#include "Hull.h"
#include "Module.h"
#include "Constants.h"
#include "Text.h"


class Space_Ship
{
private:
	SDL_Rect *position;	//Where the ship is position (top-left corner)
	Hull *hull_layer[SHIP_HEIGHT][SHIP_WIDTH];
	Module *module_layer[SHIP_HEIGHT][SHIP_WIDTH];
	int selected_hull;
	int selected_module;
	int energy;

public:
	Space_Ship();
	Space_Ship(SDL_Renderer *ren, SDL_Texture *bg);
	~Space_Ship();

	void draw(SDL_Renderer *ren);
	bool buildHull(SDL_Renderer *ren);
	void swapHull(SDL_Renderer *ren, int x, int y, int type);
	bool buildModules(SDL_Renderer *ren);
	void swapModule(SDL_Renderer *ren, int x, int y, int type);
	void setPosition(int x, int y);
	void onMouseEvent(SDL_Event event);
	void resetEnergy();
	void attack(int posX, int posY, int dmg);
	void activate(Space_Ship *target);
};