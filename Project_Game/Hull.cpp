#include "Hull.h"


Hull::Hull()
{
	//Not in use
}

Hull::Hull(SDL_Renderer *rend, SDL_Rect src, SDL_Rect dst, string ico, int arm, int spAtk, int spDef, bool isLoc, bool isEmp, bool isPla)
{
	ren = rend;

	icon = IMG_LoadTexture(ren, ico.c_str());
	srcRect = new SDL_Rect(src);
	dstRect = new SDL_Rect(dst);
	armor = arm;
	specialAttack = spAtk;
	specialDefence = spDef;
	isLocked = isLoc;
	isEmpty = isEmp;
	isPlaceable = isPla;
	numAnchors = 0;
}

Hull::~Hull()
{
}

string Hull::networkConstruction(){
	stringstream hull;
	hull  << srcRect->h << "/" << srcRect->w <<"/"<< srcRect->x 
		<< "/"<< srcRect->y << "/"<< dstRect->h << "/" << dstRect->w <<"/"<< dstRect->x 
		<< "/"<< dstRect->y << "/"<< armor << "/"<< specialAttack << "/"<< specialDefence 
		<< isLocked << "/"<< isEmpty << "/"<<  numAnchors;
	//TODO:: hva skal sendes ...
return hull.str();
}

void Hull::drawBuild()
{
	if (!isEmpty || (isEmpty && isPlaceable))
	{
		SDL_RenderCopy(ren, icon, srcRect, dstRect);
	}
}

void Hull::draw()
{
	if (!isEmpty)
	{
		SDL_RenderCopy(ren, icon, srcRect, dstRect);
	}
}

SDL_Rect Hull::getSource()
{
	return SDL_Rect(*srcRect);
}

SDL_Rect Hull::getDestination()
{
	return SDL_Rect(*dstRect);
}

hull_action Hull::onMouseClick(SDL_Event event)
{
	int mouseX = event.button.x;
	int mouseY = event.button.y;
	hull_action action = HUL_NOTHING;
	
	if (isMouseOver(event)) {
		cout << "[BUILD-CLICK]: ";
		if (isLocked && !isEmpty)
		{
			cout << "This tile is LOCKED!";
		}
		else if (isEmpty && isPlaceable)
		{
			cout << "This tile is EMPTY!";
			action = HUL_PLACE;
		}
		else if (!isEmpty)
		{
			cout << "This hull is OPEN!";
			action = HUL_REMOVE;
		}
		else
		{
			cout << "This tile is NOT VISIBLE!";
		}
		cout << endl;
	}

	return action;
}

bool Hull::isMouseOver(SDL_Event event)
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

void Hull::setLocked(bool state)
{
	if (!isEmpty)
	{
		isLocked = state;
	}
}

void Hull::setEmpty(bool state)
{
	isEmpty = state;
}

void Hull::setPlaceable(bool state)
{
	if (isEmpty)
	{
		if (state)
		{
			numAnchors += 1;
			isPlaceable = state;
		}
		else
		{
			numAnchors -= 1;
			if (numAnchors <= 0)
			{
				isPlaceable = state;
			}
		}
	}
}

int Hull::getAnchors()
{
	return numAnchors;
}

void Hull::setAnchors(int num)
{
	numAnchors = num;
}

bool Hull::isNotUsed()
{
	return isEmpty;
}

void Hull::setPosition(int x, int y)
{
	dstRect->x += x;
	dstRect->y += y;
}

int Hull::getType()
{
	int type = -1;

	//Check if an existing module
	if (armor)
	{
		if (specialAttack)
		{
			type = hull_type(ELECTRICAL);
		}
		else if (specialDefence)
		{
			type = hull_type(REINFORCED);
		}
		else
		{
			type = hull_type(NORMAL);
		}
	}

	return type;
}