#include <iostream>

#include "Module.h"



Module::Module()
{
}

Module::Module(SDL_Renderer *rend, SDL_Rect src, SDL_Rect dst, string ico, int namId, int maxHp, int acc, int hullId, int reqPow, int dmg, int disPow, int act)
{
	ren = rend;

	runningAnimation = false;

	icon = IMG_LoadTexture(ren, ico.c_str());
	srcRect = new SDL_Rect(src);
	dstRect = new SDL_Rect(dst);

	srcRocket = new SDL_Rect();
	srcRocket->w = ROCKET_SIZE_W;
	srcRocket->h = ROCKET_SIZE_H;
	srcRocket->x = 0;
	srcRocket->y = 0;

	dstRocket = new SDL_Rect();
	dstRocket->w = ROCKET_SIZE_W;
	dstRocket->h = ROCKET_SIZE_H;
	dstRocket->x = dstRect->x + (dstRect->w / 2) - (dstRocket->w / 2);
	dstRocket->y = dstRect->y + (dstRect->h / 2) - (dstRocket->h / 2);

	srcEffect = new SDL_Rect();
	srcEffect->w = SHIELD_SIZE;
	srcEffect->h = SHIELD_SIZE;
	srcEffect->x = 0;
	srcEffect->y = 0;

	nameId = namId;
	maxHealth = maxHp;
	currentPower = 0;
	disabled = 0;
	disablePower = disPow;
	accuracy = acc;
	active = false;
	hovered = false;
	held = false;
	changedPower = 0;

	defence = 0;
	damage = dmg;
	activeTurns = act;
	requiredPower = reqPow;
	activeLostOnHit = 1;
	
	string icoStr = DIR_MISC + "Energy.png";
	iconPower = IMG_LoadTexture(ren, icoStr.c_str());

	icoStr = DIR_MISC + "Energy_Empty.png";
	iconPowerEmpty = IMG_LoadTexture(ren, icoStr.c_str());

	icoStr = DIR_MODULES + "Highlight.png";
	iconHighlight = IMG_LoadTexture(ren, icoStr.c_str());

	if (dmg) {
		//Turret uses rockets
		icoStr = DIR_MISC + "Rocket.png";

		//Set data that are specific to hull
		if (hullId == ELECTRICAL)
		{
			damage += 3;
			maxHealth -= 3;
		}
		else if (hullId == REINFORCED)
		{
			maxHealth += 3;
			requiredPower += 1;
		}
	}
	else if (act) {
		//Shields uses shiled bubbles
		icoStr = DIR_MISC + "Shield_Barrier.png";

		//Set data that are specific to hull
		if (hullId == ELECTRICAL)
		{
			activeTurns += 1;
			activeLostOnHit += 3;
		}
		else if (hullId == REINFORCED)
		{
			activeTurns -= 1;
			activeLostOnHit -= 0;
		}
	}

	currentHealth = maxHealth;

	string tmpTxt = numToStr(currentHealth) + "/" + numToStr(maxHealth);
	healthText = new Text(ren, tmpTxt.c_str(), DIR_FONTS + "Custom_Orange.png");
	healthText->setPosition(dstRect->x + 4, dstRect->y + 4);

	iconActivate = IMG_LoadTexture(ren, icoStr.c_str());
	
	activeLeft = activeTurns;

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
	int add = 0;

	if (!active)
	{
		add = ((currentPower < requiredPower) ? 1 : 0);
		currentPower += add;
		changedPower += add;
	}

	return add;
}

bool Module::getActive(){
	return active;
}

bool Module::removeEnergy()
{
	//Remove power if possible
	int remove = 0;

	if (!active)
	{
		if (changedPower > 0)
		{
			remove = ((currentPower > 0) ? 1 : 0);
			currentPower -= remove;
		}
		else
		{
			cout << "You tried to remove energy from last round (or it is empty)!\n";
		}
	}

	return remove;
}

void Module::resetEnergy()
{
	//Make sure energy from last round can't be moved
	changedPower = 0;

	//Remove if used
	if (currentPower == requiredPower)
	{
		if (hasTarget() || getType() == SHIELD)
		{
			currentPower = 0;
			clearTarget();
		}
	}
	
	//If effect is active
	if (active)
	{
		//Decrease effect lifetime
		if (activeLeft)
		{
			activeLeft--;
			cout << "[SHIELD]: Up for " << activeLeft << " more turns!\n";
		}

		//Check if effect wore off
		if (!activeLeft)
		{
			cout << "[SHIELD]: Wore off!\n";
			activeLeft = activeTurns;
			active = false;
		}
	}
}

int Module::getCurrentEnergy(){
	return currentPower;
}

int Module::getReqPower(){
	return requiredPower;
}

bool Module::canActivate()
{
	//Check if able to use
	bool charged = ((currentPower == requiredPower) ? true : false);
	return charged;
}

void Module::setHeld(bool state)
{
	held = state;
	//cout << "I'm now: " << ((state) ? "HELD" : "NOT HELD") << "!\n";
}

bool Module::getHeld()
{
	return held;
}

void Module::clearTarget()
{
	if (targetX != -1 || targetPosX != -1)
	{
		//cout << "Cleared target!\n";
		targetX = -1;
		targetY = -1;
		targetPosX = -1;
		targetPosY = -1;
	}
}

void Module::setTarget(int x, int y, int posX, int posY)
{
	if ((targetX != x || targetY != y) && !activeTurns)
	{
		cout << "Target (" << x << ", " << y << ", " << posX << ", " << posY << ")\n";
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

void Module::setTargetLineToMouse(int mouseX, int mouseY)
{
	//If no target
	if (targetX == -1 && !activeTurns)
	{
		//And a change occured
		if (targetX != mouseX || targetY != mouseY)
		{
			//Follow mouse
			targetPosX = mouseX - (dstRect->w / 2);
			targetPosY = mouseY - (dstRect->h / 2);
			//cout << "Target is following the mouse!\n";
		}
	}
}

bool Module::hasTarget()
{
	return ((targetX != -1 || getType() == SHIELD) ? true : false);
}

void Module::drawInterface()
{
	if (currentHealth > 0)
	{
		//Draw active effect
		if (active)
		{
			//(This will only run for shields)
			SDL_Rect *dstEffect = new SDL_Rect();
			dstEffect->w = SHIELD_SIZE;
			dstEffect->h = SHIELD_SIZE;
			dstEffect->x = dstRect->x + (dstRect->w / 2) - (srcEffect->w / 2);
			dstEffect->y = dstRect->y + (dstRect->h / 2) - (srcEffect->h / 2);

			SDL_RenderCopy(ren, iconActivate, srcEffect, dstEffect);
		}

		//Draw highlight if nessesary
		if (hovered || held)
		{
			SDL_RenderCopy(ren, iconHighlight, srcRect, dstRect);
		}

		//Draw misc interface
		healthText->draw();

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

		//Draw target line
		if (targetPosX != -1)
		{
			//Snap to target
			int x1 = dstRect->x + (dstRect->w / 2);
			int y1 = dstRect->y + (dstRect->h / 2);
			int x2 = targetPosX + (dstRect->w / 2);
			int y2 = targetPosY + (dstRect->h / 2);

			SDL_SetRenderDrawColor(ren, 255, 255, 255, 1);
			SDL_RenderDrawLine(ren, x1, y1, x2, y2);
		}

		//Show rocket if animation is running
		if (runningAnimation)
		{
			//Draw rocket
			if (mirrorRocket)
			{
				SDL_RenderCopyEx(ren, iconActivate, srcRocket, dstRocket, NULL, NULL, SDL_FLIP_HORIZONTAL);
			}
			else
			{
				SDL_RenderCopy(ren, iconActivate, srcRocket, dstRocket);
			}
		}
	}
}

bool Module::isShielding()
{
	bool shield = false;
	//Is shield, active and alive?
	if (active && activeTurns && currentHealth)
	{
		shield = true;

		//Countdown durability
		for (int i = 0; i < activeLostOnHit; i++)
		{
			resetEnergy();
		}
	}
	return shield;
}

bool Module::runRocketAnimation(Module *end)
{
	//Calculate distance and direction
	int x2, y2;
	end->getPosition(x2, y2);

	int distX = (x2 + (dstRect->w / 2)) - (dstRocket->x + (dstRocket->w / 2));
	int distY = (y2 + (dstRect->h / 2)) - (dstRocket->y + (dstRocket->h / 2));

	//Start animation if not running
	if (!runningAnimation)
	{
		dstRocket->x = dstRect->x + (dstRect->w / 2) - (dstRocket->w / 2);
		dstRocket->y = dstRect->y + (dstRect->h / 2) - (dstRocket->h / 2);
		rocketSpeed = 1;

		//Mirror if going negative x
		mirrorRocket = ((x2 < dstRect->x) ? true : false);

		runningAnimation = true;
		cout << "[ANIMATION]: Started!\n";
	}
	else
	{
		//Check if approximatly at the goal
		if ((distX > -rocketSpeed && distX < rocketSpeed) &&
			(distY > -rocketSpeed && distY < rocketSpeed))
		{
			runningAnimation = false;
			cout << "[ANIMATION]: Ended!\n";
		}
		else
		{
			//cout << "[ANIMATION]: Calculating!\n";

			//Check x movement
			if (distX <= -rocketSpeed || distX >= rocketSpeed)
			{
				dstRocket->x += ((distX >= 0) ? rocketSpeed : -rocketSpeed);
			}

			//Check y movement
			if (distY >= -rocketSpeed || distY <= rocketSpeed)
			{
				dstRocket->y += ((distY >= 0) ? rocketSpeed : -rocketSpeed);
			}

			//Speed it up if possible
			if (rocketSpeed < ROCKET_MAX_SPEED)
			{
				rocketSpeed++;
			}
		}
	}

	//Has the animation finished?
	return !runningAnimation;
}

string Module::registerAttack(int x, int y)
{
	string attack = "";

	if (getType() == TURRET)
	{
		attack = "Rocket " + to_string(x) + " " + to_string(y) + " " + to_string(targetX) + " " + to_string(targetY);
	}
	else if (getType() == SHIELD)
	{
		attack = "Shield " + to_string(x) + " " + to_string(y);
	}

	return attack;
}

int Module::getType()
{
	return (nameId - 1);
}

void Module::setActive()
{
	if (activeTurns && currentPower == requiredPower)
	{
		active = true;
	}
}

int Module::getChangedEnergy()
{
	return changedPower;
}

int Module::getDamage()
{
	return damage;
}

int Module::getDefence()
{
	return defence;
}

int Module::getActiveLeft()
{
	return activeLeft;
}