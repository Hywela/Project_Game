#pragma once
#include "module.h"



class Module_Shield : public Module
{
private:
	int requiredPower;	//Power required to use
	int charges;		//How many hits it can resist

public:
	Module_Shield();
	Module_Shield(string ico, int namId, int maxHp, int acc, int reqPow, int cha);
	~Module_Shield();
};