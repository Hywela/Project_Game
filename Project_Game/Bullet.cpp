#include "Bullet.h"


Bullet::Bullet(SDL_Renderer *rend, int x , int y)
{
	ren = rend;
	string bgStr = DIR_HULLS + "Normal.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());
	start_postionX =x; start_postionY = y;
	srcRect = new SDL_Rect();
	srcRect->x = 0;
	srcRect->y = 0;
	srcRect->w = 10;
	srcRect->h = 10;
	dstRect = new SDL_Rect();
	dstRect->x = x;
	dstRect->y = y;
	dstRect->w = 20;
	dstRect->h = 20;
}


Bullet::~Bullet()
{
}
void Bullet::shoot(){

	dstRect++;	
}
bool Bullet::reachedDest(){
	if (srcRect->x >= start_postionX+20)
		return true;
	else return false;

}
void Bullet::draw(){

	//srcRect->y++;
	//Draw background
	SDL_RenderCopy(ren, background, srcRect, dstRect);

}
