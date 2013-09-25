#include <iostream>

#include "Module_Gun.h"



Module_Gun::Module_Gun()
{
}

Module_Gun::Module_Gun(SDL_Renderer *ren, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow, int dmg, int disPow) : Module(ren, src, dst, ico, namId, maxHp, acc)
{
	requiredPower = reqPow;
	damage = dmg;
	disablePower = disPow;
}

Module_Gun::~Module_Gun()
{
}

void Module_Gun::printData()
{
	int tmp_dummy;

	Module::printData();
	cout << requiredPower
		 << "\nDamage: " << damage
		 << "\nDisable Power: " << disablePower << endl;
	system("pause");
}