#include "Module_Shield.h"



Module_Shield::Module_Shield()
{
}

Module_Shield::Module_Shield(SDL_Renderer *ren, string ico, int namId, int maxHp, int acc, int reqPow, int cha) : Module(ren, ico, namId, maxHp, acc)
{
	requiredPower = reqPow;
	charges = cha;
}

Module_Shield::~Module_Shield()
{
}