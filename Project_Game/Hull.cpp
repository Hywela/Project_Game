#include "Hull.h"


Hull::Hull()
{
	//Not in use
}

Hull::Hull(string ico, int arm, int spAtk, int spDef)
{
	//icon = new SDL_Texture();
	armor = arm;
	specialAttack = spAtk;
	specialDefence = spDef;
}

Hull::~Hull()
{

}

void Hull::draw(SDL_Renderer *ren)
{
	//SDL_RenderCopy(ren, icon, srcRect, dstRect);
}