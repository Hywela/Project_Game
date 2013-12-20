#include "Combat.h"
/* Notes:
	- Send ships from database with current stats and statuses
	  to be able to continue from a previously started game.
*/


Combat::Combat()
{
}
Combat::Combat(Space_Ship *yourShip, Space_Ship *enemyShip, bool turnStart, SDL_Renderer *rend, SDL_Window *wind , Network *instanceOfServer){
	//Copy renderer and window
	ren = rend;
	win = wind;
	server = instanceOfServer;
	//Set up button properties
	int winH, winW;
	SDL_GetWindowSize(win, &winW, &winH);

	int btnWidth = 140;
	int btnHeight = 30;
	int startY = 220;
	int offsetY = 20;
	int btnX = (winW / 2);
	int btnY = (winH / 8) - (btnHeight / 2);
	float scaleX = btnX;
	float scaleY = btnY;

	buttons.push_back(new Button(ren, DIR_BUTTONS + "Green.png", DIR_FONTS + "Custom_Orange.png", btnX - btnWidth - 20, btnY, "End turn", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Red.png", DIR_FONTS + "Custom_White.png", btnX + 20, btnY, "Quit", btnWidth, btnHeight));

	//Create texture from image
	string bgStr = DIR_BACKGROUNDS + "Space.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set game properties
	you = yourShip;
	enemy = enemyShip;
	yourTurn = turnStart;
	surrender = false;

	//Position ships
	int ship1X = (winW / 4) - (((64 + 2) * 5) / 2);
	int ship2X = ((winW / 4) * 3) - (((64 + 2) * 5) / 2);
	int shipCentreY = 32 + (winH / 2) - (((64 + 2) * 5) / 2);

	you->setPosition(ship1X, shipCentreY);
	enemy->setPosition(ship2X, shipCentreY);

	//Make interface
	statusEnergyLeft = new Text(ren, "Energy: ?/?");
	int statusCentreX = (winW / 2) - (statusEnergyLeft->getWidth() / 2) - 8;
	int statusY = 14;
	statusEnergyLeft->setPosition(statusCentreX, statusY);

	//Set targets
	you->setTarget(enemy);
	enemy->setTarget(you);

	//Force update
	draw();

	//Ship health, calculates as a overall from modules. (Shields etc.)
	bool youAlive = true;
	bool enemyAlive = true;

	if (instanceOfServer == NULL)
	{
		//Set enemy computer
		enemy->setComputer(true);
		ai = new Ai(enemyShip, you, 1, 1);
	}

	//While both still alive
	while(youAlive && enemyAlive && !surrender)
	{
		//Wait for animations to finish
		doneAnimating = false;

		//Preform turn
		if (instanceOfServer == NULL)
		{
			//Make local battle configs
			((yourTurn) ? makeMoves() : listenForMoves());
		}
		else
		{
			//Make server battle config
			((yourTurn) ? makeMovesPVP() : listenForMovesPVP());
		}

		//Check if combat is over
		youAlive = !you->isDead();
		enemyAlive = !enemy->isDead();
	}

	if (surrender)
	{
		cout << ((yourTurn) ? "The opponent" : "You") << " have surrendered!\n";
	}

	//Game done, heal restore ships
	you->restore();
	if (instanceOfServer == NULL)
	{
		enemy->restore();
	}
}

void Combat::makeMovesPVP()
{
	//Use normal interface for setting up moves
	makeMoves();

	//When done, post to server
	yourTurn = true;
	setupAttacksPVP();
	yourTurn = false;
}
void Combat::makeMoves()
{

	//For all attacks registered
	cout << "Enemy moves:\n";
	for (int i = 0; i < enemyAction.size(); i++)
	{
		//Draw animation
		cout << enemyAction[i] << endl;
		if (enemyAction[i].find("Power") == string::npos)
		{
			playAnimation(enemyAction[i]);
		}
	}
	enemyAction.clear();
	doneAnimating = true;

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
					setupAttacks();
					yourTurn = false;
				}
				else if (hit == "Quit")
				{
					cout << "Quit game!\n";
					surrender = true;
					yourAction.push_back("Surrender");
					yourTurn = false;
				}
			}

			//Connect events
			you->onMouseEvent(event);
		}
	}
}

void Combat::listenForMovesPVP(){
	//For all attacks registered
	cout << "Your moves:\n";
	for (int i = 0; i < yourAction.size(); i++){
		//Draw animation
		cout << yourAction[i] << endl;
		if (yourAction[i].find("Power") == string::npos){
			playAnimation(yourAction[i]);
		}
	}
	yourAction.clear();
	doneAnimating = true;

	//Force update
	draw();

	setupAttacksPVP();
}
void Combat::listenForMoves(){
	//For all attacks registered
	cout << "Your moves:\n";
	for (int i = 0; i < yourAction.size(); i++)
	{
		//Draw animation
		cout << yourAction[i] << endl;
		if (yourAction[i].find("Power") == string::npos)
		{
			playAnimation(yourAction[i]);
		}
	}
	yourAction.clear();
	doneAnimating = true;

	//Force update
	draw();

	//Recieve answer
	ai->aiActions();

	setupAttacks();

	yourTurn = true;
}

void Combat::draw()
{
	//Reset screen
	SDL_RenderClear(ren);

	//Draw background
	SDL_RenderCopy(ren, background, NULL, NULL);

	//Draw ships
	if (doneAnimating) {
		//Watch your interface
		enemy->draw();
		you->draw();
	}
	else
	{
		//Watch opponents actions ontop
		((yourTurn) ? you->draw() : enemy->draw());
		((yourTurn) ? enemy->draw() : you->draw());
	}

	//Draw buttons
	if (yourTurn && doneAnimating) {
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

void Combat::playAnimation(string attackCode)
{
	//Animation data
	Space_Ship *attacker = ((yourTurn) ? enemy : you);
	Space_Ship *defender = ((yourTurn) ? you : enemy);
	Module *attackerModule = NULL;
	Module *defenderModule = NULL;
	vector <string> args;
	string currentArg = "";

	//Decode attack
	for (char &c : attackCode)
	{
		if (c != ' ')
		{
			//Build string
			currentArg += c;
		}
		else
		{
			//Store string and reset
			args.push_back(currentArg);
			currentArg = "";
		}
	}

	//Store last argument
	args.push_back(currentArg);

	int x1 = atoi(args[1].c_str());
	int y1 = atoi(args[2].c_str());
	attackerModule = attacker->getModule(y1, x1);

	//Use energy
	attackerModule->setActive();
	attackerModule->resetEnergy();

	if (args[0] == "Rocket")
	{
		int x2 = atoi(args[3].c_str());
		int y2 = atoi(args[4].c_str());
		defenderModule = defender->getModule(y2, x2);

		//Run animation sequence
		bool donePlaying = false;
		while (!donePlaying)
		{
			//Draw world
			draw();

			//Draw animation
			donePlaying = attackerModule->runRocketAnimation(defenderModule);
		}

		//Execute damage
		int rocketDamage = attackerModule->getDamage();
		((yourTurn) ? you->attack(x2, y2, rocketDamage) : enemy->attack(x2, y2, rocketDamage));
	}
}

void Combat::setupAttacksPVP()
{
	if (yourTurn)
	{
		stringstream sendAttack; sendAttack << "f ";
		//Send how many events
		sendAttack << yourAction.size();

		//Send all events 
		for (int i = 0; i < yourAction.size(); i++){
			sendAttack <<"/" << yourAction[i];
		}

		server->send(sendAttack.str());
		cout <<"\n" <<sendAttack.str();
	}
	else
	{
		cout << "Waiting for opponent!\n";
		
		//Wait for count to be read
		string attackStr = server->reciveString(0);
		
		if (attackStr.length() > 3) { //TODO:: Fiks so it wont have to loop 
			cout << "\n attac  "<< attackStr;
			string item;
			stringstream ss(attackStr.substr(+2));

			//Read all events
			std::getline(ss, item, '/');// get the number of attacks split it on delim /
			int numberOfAttacks = atoi(item.c_str());
			cout << "\n count " << numberOfAttacks;

			for (int i =0; i< numberOfAttacks; i++){ // get the attacks split it on delim /
				std::getline(ss, item, '/');
				enemyAction.push_back(item);
				cout << "\n ITEM " << item;
			}

			//Update ship
			prepareShip();
			yourTurn = true;
		}
	}
}
void Combat::setupAttacks()
{
	Space_Ship *attacker = ((yourTurn) ? you : enemy);

	vector <string> attacks = attacker->activate();
	if (yourTurn)
	{
		yourAction = attacks;
	}
	else
	{
		enemyAction = attacks;
	}
}

void Combat::prepareShip(){
	for (int i = 0; i < enemyAction.size(); i++){
		//Draw animation
		cout << enemyAction[i] << endl;
		vector <string> args;
		string currentArg = "";

		//Decode attack
		for (char &c : enemyAction[i]){
			if (c != ' '){
				//Build string
				currentArg += c;
			}
			else{
				//Store string and reset
				args.push_back(currentArg);
				currentArg = "";
			}
		}//end - for(...)

		//Store last argument
		args.push_back(currentArg);

		int x1 = atoi(args[1].c_str());
		int y1 = atoi(args[2].c_str());
		Module *currentModule = enemy->getModule(y1, x1);

		if(args[0] == "Power"){
			currentModule->addEnergy();
		}
		else if(args[0] == "Rocket"){
			int x2 = atoi(args[3].c_str());
			int y2 = atoi(args[4].c_str());
			Module *target = enemy->getModule(y2, x2);

			int posX,posY;
			target->getPosition(posX,posY);
			currentModule->setTarget(x2, y2, posX, posY);
		}
		else if (args[0] == "Surrender") {
			surrender = true;
		}
	}//end - for(...)
}