#include <iostream>

#include "Module.h"



Module::Module()
{
}

Module::Module(SDL_Renderer *ren, string ico, int namId, int maxHp, int acc)
{
	icon = SDL_CreateTextureFromSurface(ren, NULL);
	nameId = namId;
	maxHealth = maxHp;
	currentHealth = maxHealth;
	currentPower = 0;
	disabled = 0;
	accuracy = acc;
}

Module::~Module()
{
}



void Module::printData()
{
	cout << "Icon: ";
	if (icon == NULL)
	{
		cout << "<none>";
	}
	cout << "\nHealth: " << currentHealth << " / " << maxHealth
		 << "\nAccuracy: " << accuracy << "%"
		 << "\nDiabled: " << disabled << " Turns"
		 << "\nPower: " << currentPower << " / ";
}

int Module::getMaxHealth()
{
	return maxHealth;
}

int Module::getCurrentHealth()
{
	return currentHealth;
}

void Module::onHit(int dmg)
{
	currentHealth -= dmg;
	cout << "... did " << dmg << " damage";

	if (currentHealth < 0)
	{
		cout << " (" << -currentHealth << " overkill!)";
		currentHealth = 0;
	}
	cout << ".\n";
}

void Module::draw(SDL_Renderer *ren)
{

}