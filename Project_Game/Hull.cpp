#include "Hull.h"


Hull::Hull()
{
	//Not in use
}

Hull::Hull(string ico, int arm, int spAtk, int spDef)
{
	icon = new SDL_Surface();
	armor = arm;
	specialAttack = spAtk;
	specialDefence = spDef;
}

Hull::~Hull()
{

}