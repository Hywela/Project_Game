#include "Module_Shield.h"



Module_Shield::Module_Shield()
{
}

Module_Shield::Module_Shield(SDL_Renderer *ren, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow, int cha) : Module(ren, src, dst, ico, namId, maxHp, acc)
{
	requiredPower = reqPow;
	charges = cha;
}

Module_Shield::~Module_Shield()
{
}