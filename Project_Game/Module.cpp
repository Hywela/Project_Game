#include <iostream>

#include "Module.h"



Module::Module()
{
}

Module::Module(SDL_Renderer *ren, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow, int dmg, int disPow)
{
	icon = IMG_LoadTexture(ren, ico.c_str());
	srcRect = new SDL_Rect(src);
	dstRect = new SDL_Rect(dst);

	nameId = namId;
	maxHealth = maxHp;
	currentHealth = maxHealth;
	currentPower = 0;
	disabled = 0;
	accuracy = acc;
	string tmpTxt = numToStr(currentHealth) + "/" + numToStr(maxHealth);
	healthText = new Text(ren, tmpTxt.c_str(), DIR_FONTS + "Custom_Orange.png");
	healthText->setPosition(dstRect->x + 4, dstRect->y + 4);

	string icoStr = DIR_MISC + "Energy.png";
	iconPower = IMG_LoadTexture(ren, icoStr.c_str());

	icoStr = DIR_MISC + "Energy_Empty.png";
	iconPowerEmpty = IMG_LoadTexture(ren, icoStr.c_str());

	requiredPower = reqPow;
	damage = dmg;
	disablePower = disPow;
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

	//Update string
	string tmpTxt = numToStr(currentHealth) + "/" + numToStr(maxHealth);
	healthText->setText(tmpTxt.c_str());
}

void Module::draw(SDL_Renderer *ren)
{
	if (currentHealth > 0)
	{
		//Draw module tile
		SDL_RenderCopy(ren, icon, srcRect, dstRect);

		//Draw misc interface
		healthText->draw(ren);

		SDL_Rect *srcOrb = new SDL_Rect();
		srcOrb->w = 10;
		srcOrb->h = 10;
		srcOrb->x = 0;
		srcOrb->y = 0;

		SDL_Rect *dstOrb = new SDL_Rect();
		dstOrb->w = 10;
		dstOrb->h = 10;
		dstOrb->x = dstRect->x + 4;
		dstOrb->y = dstRect->y + dstRect->h - dstOrb->h - 4;

		//Draw energy load
		for (int i = 0; i < requiredPower; i++)
		{
			dstOrb->x = dstRect->x + 4 + ((dstOrb->w + 1) * i);
			if (i < currentPower)
			{
				//Draw filled
				SDL_RenderCopy(ren, iconPower, srcOrb, dstOrb);
			}
			else
			{
				//Draw empty
				SDL_RenderCopy(ren, iconPowerEmpty, srcOrb, dstOrb);
			}
		}
	}
}

SDL_Rect Module::getSource()
{
	return SDL_Rect(*srcRect);
}

SDL_Rect Module::getDestination()
{
	return SDL_Rect(*dstRect);
}

module_action Module::onMouseClick(SDL_Event event)
{
	int mouseX = event.button.x;
	int mouseY = event.button.y;
	module_action action = MOD_NOTHING;
	
	if (isMouseOver(event)) {
		cout << "[BUILD-CLICK]: ";
		if (maxHealth > 0)
		{
			cout << "This module is NOT EMPTY";
			action = MOD_REMOVE;
		}
		else
		{
			cout << "This tile is EMPTY!";
			action = MOD_PLACE;
		}
		cout << endl;
	}

	return action;
}

bool Module::isMouseOver(SDL_Event event)
{
	bool isOver = false;
	int mouseX = event.button.x;
	int mouseY = event.button.y;

	//Check if inside x-boundry
	if (mouseX >= dstRect->x && mouseX <= dstRect->x + dstRect->w)
	{
		//Check if inside y-boundry
		if (mouseY >= dstRect->y && mouseY <= dstRect->y + dstRect->h)
		{
			isOver = true;
		}
	}

	//Return result
	return isOver;
}

void Module::setPosition(int x, int y)
{
	dstRect->x += x;
	dstRect->y += y;
	healthText->setPosition(dstRect->x + 4, dstRect->y + 4);
}

bool Module::addEnergy()
{
	//Add power if possible
	int add;
	add = ((currentPower < requiredPower) ? 1 : 0);
	currentPower += add;
	return add;
}

bool Module::removeEnergy()
{
	//Remove power if possible
	int remove;
	remove = ((currentPower > 0) ? 1 : 0);
	currentPower -= remove;
	return remove;
}

void Module::resetEnergy()
{
	//Remove if used
	if (currentPower == requiredPower)
	{
		currentPower = 0;
	}
}

bool Module::activate()
{
	//Check if able to use
	bool charged = false;
	int charge = ((currentPower == requiredPower) ? currentPower : 0);
	if (charge)
	{
		resetEnergy();
		charged = true;
	}
	return charged;
}