#pragma once
#include "Hull.h"
#include "Module.h"
#include "Module_Gun.h"
#include "Module_Shield.h"
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

public:
	Space_Ship();
	Space_Ship(SDL_Renderer *ren, SDL_Texture *bg);
	~Space_Ship();

	void draw(SDL_Renderer *ren);
	bool buildHull(SDL_Renderer *ren);
	void swapHull(SDL_Renderer *ren, int x, int y, int type);
	bool buildModules(SDL_Renderer *ren);
	void swapModule(SDL_Renderer *ren, int x, int y, int type);
};

