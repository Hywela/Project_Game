#include "Space_Ship.h"



Space_Ship::Space_Ship()
{
	//Load space ship
}

Space_Ship::~Space_Ship()
{
}

void Space_Ship::draw(SDL_Renderer *ren)
{
	//Draw hulls
	for (int y = 0; y < SHIP_HEIGTH; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			hull_layer[y][x]->draw(ren);
		}
	}
	
	//Draw modules
}