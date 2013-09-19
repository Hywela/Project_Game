#include "Module_Gun.h"
#include <iostream>
using namespace std;



Module_Gun::Module_Gun()
{
}

Module_Gun::Module_Gun(string ico, int namId, int maxHp, int acc, int reqPow, int dmg, int disPow) : Module(ico, namId, maxHp, acc)
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