#pragma once
#include "Hull.h"
#include "Module.h"
#include "Module_Gun.h"
#include "Module_Shield.h"



const int SHIP_WIDTH = 3;
const int SHIP_HEIGTH = 3;



class Space_Ship
{
private:
	Hull *hull_layer[SHIP_HEIGTH][SHIP_WIDTH];
	Module *module_layer[SHIP_HEIGTH][SHIP_WIDTH];

public:
	Space_Ship();
	~Space_Ship();

	void draw(SDL_Renderer *ren);
};

