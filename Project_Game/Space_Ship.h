#pragma once
#include "Hull.h"
#include "Module.h"
#include "Constants.h"
#include "Text.h"
#include <vector>


class Space_Ship
{
private:
	SDL_Renderer *ren;
	SDL_Rect *position;	//Where the ship is position (top-left corner)
	Hull *hull_layer[SHIP_HEIGHT][SHIP_WIDTH];
	Module *module_layer[SHIP_HEIGHT][SHIP_WIDTH];
	hull_type selected_hull;
	module_type selected_module;
	int energy;
	int energyMax;
	bool computer;
	bool isDestroyed;
	bool build_hull;
	bool build_module;
	Space_Ship *target;
	Text *tutorialHull;
	Text *tutorialModule;
	Text *tutorialHullSelected;
	Text *tutorialModuleSelected;
	Text *titleText;
	SDL_Texture *buildBackground;
	SDL_Rect *tutorialTextureSrc;
	SDL_Rect *tutorialTextureDst;
	vector <SDL_Texture*> hullTextures;
	vector <SDL_Texture*> moduleTextures;

public:
	Space_Ship();
	Space_Ship(SDL_Renderer *rend, SDL_Texture *bg, string title = "");
	~Space_Ship();

	void draw();
	bool buildHull();
	void swapHull(int x, int y, int type);
	bool buildModules();
	void swapModule(int x, int y, int type);
	void setPosition(int x, int y);
	void onMouseEvent(SDL_Event event);
	void resetEnergy();
	void attack(int posX, int posY, int dmg);
	vector <string> activate();
	void setTarget(Space_Ship *tar);
	void getHoveredModule(SDL_Event event, int &x1, int &y1, int &posX, int &posY);
	void setComputer(bool state);
	void getEnergyLevel(int &cur, int &tot);
	void checkModuleHealth();
	bool isDead();
	void restore();
	Module* getModule(int row, int collumn);
	Hull* getHull(int row, int collumn);
	void drawBuild();
};