#include "Combat.h"
/* Notes:
	- Send ships from database with current stats and statuses
	  to be able to continue from a previously started game.
*/


Combat::Combat()
{
}

Combat::Combat(Space_Ship *yourShip, Space_Ship *enemyShip, bool youStart, SDL_Renderer *rend, SDL_Window *wind)
{
	//Copy renderer and window
	ren = rend;
	win = wind;

	//Set up button properties
	int winH, winW;
	SDL_GetWindowSize(win, &winW, &winH);

	int btnWidth = 140;
	int btnHeight = 30;
	int startY = 220;
	int offsetY = 20;
	int btnX = (winW / 2) - (btnWidth / 2);
	int btnY = (winH / 8) - (btnHeight / 2);
	float scaleX = btnX;
	float scaleY = btnY;

	buttons.push_back(new Button(ren, DIR_BUTTONS + "Green.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "End turn", btnWidth, btnHeight));

	//Create texture from image
	string bgStr = DIR_BACKGROUNDS + "Space.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set game properties
	you = yourShip;
	enemy = enemyShip;
	yourTurn = youStart;

	//Position ships
	int ship1X = (winW / 4) - (((64 + 2) * 5) / 2);
	int ship2X = ((winW / 4) * 3) - (((64 + 2) * 5) / 2);
	int shipCentreY = 32 + (winH / 2) - (((64 + 2) * 5) / 2);

	you->setPosition(ship1X, shipCentreY);
	enemy->setPosition(ship2X, shipCentreY);

	//Make interface
	statusEnergyLeft = new Text(ren, "Energy: ?/?");
	int statusCentreX = (winW / 2) - (statusEnergyLeft->getWidth() / 2);
	int statusY = 100;
	statusEnergyLeft->setPosition(statusCentreX, statusY);

	//Set targets
	you->setTarget(enemy);
	enemy->setTarget(you);

	//Set enemy computer
	enemy->setComputer(true);

	//Ship health, calculates as a overall from modules. (Shields etc.)
	bool youAlive = true;
	bool enemyAlive = true;

	//While both still alive
	while(youAlive && enemyAlive)
	{
		//Preform turn
		((yourTurn) ? makeMoves() : listenForMoves());

		//Check if combat is over
		youAlive = !you->isDead();
		enemyAlive = !enemy->isDead();
	}

	//Game done, heal restore ships
	you->restore();
	enemy->restore();
}

void Combat::makeMoves()
{
	//For all attacks registered
	for (int i = 0; i < enemyAction.size(); i++)
	{
		//Draw combat
		draw();

		//Play some animation?
		cout << "- " << enemyAction[i] << endl;
	}
	enemyAction.clear();

	//Refill energy
	you->resetEnergy();

	//This players turn
	while (yourTurn)
	{
		//Draw combat
		draw();

		//Handle incomming events
		while (SDL_PollEvent(&event))
		{
			//Mouse events for the buttons
			for (int i = 0; i < buttons.size(); i++)
			{
				//Check if the mouse are hovering over any buttons
				buttons[i]->isMouseOver(event);

				//Check if it clicked it
				string hit = buttons[i]->onMouseClick(event);
				if (hit == "End turn")
				{
					cout << "Ending turn!\n";
					yourTurn = false;
					yourAction.push_back("Attack Missile 2,2");
					yourAction.push_back("Defence Shield 0,0");
				}
			}

			//Connect events
			you->onMouseEvent(event);
		}
	}

	//Activate modules
	you->activate();
}

void Combat::listenForMoves(){
	//The enemy's turn
	cout << "Waiting for enemy:\n";

	//For all attacks registered
	for (int i = 0; i < yourAction.size(); i++)
	{
		//Draw combat
		draw();

		//Play some animation?
		cout << "- " << yourAction[i] << endl;
	}
	yourAction.clear();

	//Simulate waittime
	Sleep(2000);

	//Recieve answer
	enemyAction.push_back("Attack Rocket 0,0");
	enemyAction.push_back("Defence Shield 2,2");
	cout << "Your enemy responded with:\n";
	yourTurn = true;
}

void Combat::draw()
{
	//Reset screen
	SDL_RenderClear(ren);

	//Draw background
	SDL_RenderCopy(ren, background, NULL, NULL);

	//Draw ships
	enemy->draw();
	you->draw();

	//Draw buttons
	if (yourTurn) {
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->draw();
		}
	}

	//Draw interface
	int curEn, totEn;
	you->getEnergyLevel(curEn, totEn);
	string tmpStr = "Energy: " + numToStr(curEn) + "/" + numToStr(totEn);
	statusEnergyLeft->setText(tmpStr);
	statusEnergyLeft->draw();

	//Render screen
	SDL_RenderPresent(ren);
}