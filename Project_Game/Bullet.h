#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Constants.h"
using namespace std;
class Bullet
{
public:
	SDL_Renderer *ren;
	Bullet(SDL_Renderer *rend,int x , int y);
	~Bullet();
	SDL_Texture *background;
	SDL_Rect *srcRect;
	SDL_Rect *dstRect;
	int start_postionX,start_postionY;
	int position;
    int velocity;
	void shoot();
	void draw();
	bool reachedDest();

};

