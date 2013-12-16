#include <iostream>

#include "Module.h"



Module::Module()
{
}

Module::Module(SDL_Renderer *rend, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int reqPow, int dmg, int disPow)
{
	ren = rend;

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

	icoStr = DIR_MODULES + "Highlight.png";
	iconHighlight = IMG_LoadTexture(ren, icoStr.c_str());

	icoStr = DIR_MISC + "Rocket.png";
	iconBullet = IMG_LoadTexture(ren, icoStr.c_str());

	requiredPower = reqPow;
	damage = dmg;
	disablePower = disPow;
	hovered = false;
	held = false;
	clearTarget();
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

void Module::draw(bool computer)
{
	if (currentHealth > 0)
	{
		//Draw module tile
		SDL_RenderCopy(ren, icon, srcRect, dstRect);

		//Draw highlight if nessesary
		if (hovered || held)
		{
			SDL_RenderCopy(ren, iconHighlight, srcRect, dstRect);
		}

		//Draw misc interface
		healthText->draw();

		if (!computer)
		{
			//Draw target line
			if (targetX != -1)
			{
				int x1 = dstRect->x + (dstRect->w / 2);
				int y1 = dstRect->y + (dstRect->h / 2);
				int x2 = targetPosX + (dstRect->w / 2);
				int y2 = targetPosY + (dstRect->h / 2);

				SDL_RenderDrawLine(ren, x1, y1, x2, y2);
				SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
			}

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

	hovered = isOver;

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

int Module::getCurrentEnergy(){
	return currentPower;
}

int Module::getReqPower(){
	return requiredPower;
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

void Module::setHeld(bool state)
{
	held = state;
	cout << "I'm now: " << ((state) ? "HELD" : "NOT HELD") << "!\n";
}

bool Module::getHeld()
{
	return held;
}

void Module::clearTarget()
{
	if (targetX != -1)
	{
		cout << "Cleared target!\n";
		targetX = -1;
		targetY = -1;
		targetPosX = -1;
		targetPosY = -1;
	}
}

void Module::setTarget(int x, int y, int posX, int posY)
{
	if (targetX != x || targetY != y)
	{
		cout << "Target: " << x << ", " << y << endl;
		targetX = x;
		targetY = y;
		targetPosX = posX;
		targetPosY = posY;
	}
}

void Module::getTarget(int &x, int &y, int &dmg)
{
	x = targetX;
	y = targetY;
	dmg = damage;
}

void Module::getPosition(int &posX, int &posY)
{
	posX = dstRect->x;
	posY = dstRect->y;
}

bool Module::isDead()
{
	bool isAlive = ((currentHealth > 0) ? true : false);
	return !isAlive;
}

void Module::restore()
{
	//Reset attributes
	currentHealth = maxHealth;
	currentPower = 0;
	disabled = 0;
	string tmpTxt = numToStr(currentHealth) + "/" + numToStr(maxHealth);
	healthText->setText(tmpTxt);
	hovered = false;
	held = false;
	clearTarget();
}