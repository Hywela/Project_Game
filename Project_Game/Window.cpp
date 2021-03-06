#include "Window.h"
//#define offline
#define online



Window::Window()
{
	PORT = 8881; //Standar PORT Used
	serverName = "90.149.32.155"; //Standard IP

	//Load server settings
	string tmpLine = "";
	string tmpLineValue = "";
	const int SETTING_BUFFER = 32;
	ifstream settingsFile(DIR_SETTINGS + "Server.txt");
	if (!settingsFile.fail())
	{
		while (!settingsFile.eof())
		{
			//Grab the setting line
			settingsFile >> tmpLine >> tmpLineValue;

			//Remove space at the start
			if (tmpLine.find("ip:") != string::npos)
			{
				serverName = tmpLineValue;
			}
			else if (tmpLine.find("port:") != string::npos)
			{
				PORT = atoi(tmpLineValue.c_str());
			}
		}

		settingsFile.close();
	}

	cout << "Server settings:\n - IP: " << serverName << "\n - Port: " << PORT << endl;

	inQue = false;
	//Starting SDL, initializing main
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		window_state = 1;
	}

	//Create window, check for errors
	const char *title = &GAME_NAME[0];
	win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
	if (win == nullptr)
	{
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		window_state = 2;
	}

	//Create renderer, check for errors
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		window_state = 3;
	}

	if (SDL_Init(SDL_INIT_AUDIO) == -1)
	{
        printf("Error In Init");
	}

	//Set start state
	quit = false;
	isFullscreen = false;
	playerShip = NULL;
	
}

Window::~Window()
{
	if (server != NULL)
	{
		server->send("q");
		delete server;
	}

	//Close program
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int Window::getWindowState()
{
	return window_state;
}

void Window::login()
{
	SDL_GetWindowSize(win, &winW, &winH);

	//Create texture from image, check for errors
	string bgStr = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(41000, MIX_DEFAULT_FORMAT, 2, 4096);

	//Load background music
	string musicStr = DIR_MUSIC + "Harmonic Space.wav";
	music = Mix_LoadWAV(musicStr.c_str());
	Mix_VolumeChunk(music, 10);
	Mix_PlayChannelTimed(0, music, -1, NULL); //channel, sound, numLoops, playFor

	//Set up button properties
	btnWidth = 200;
	btnHeight = 40;
	startY = 220;
	offsetY = 20;
	btnX = (winW / 2) - (btnWidth / 2);
	btnY = (winH / 3) - (btnHeight / 2);
	scaleX = btnX;
	scaleY = btnY;

	//Set up buttons
	buttonsLogin.push_back(new Button(ren, DIR_BUTTONS + "Red.png", DIR_FONTS + "Custom_White.png", btnX - (btnWidth / 2) - 10, btnY * 3, "Quit", btnWidth, btnHeight));
	buttonsLogin.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX + (btnWidth / 2) + 10, btnY * 3, "Login", btnWidth, btnHeight));

	queryLogin.push_back(new EditText(ren, DIR_EDITTEXTS + "White.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, btnWidth, btnHeight, "", "Enter username..."));
	queryLogin.push_back(new EditText(ren, DIR_EDITTEXTS + "White.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + btnHeight + 20, btnWidth, btnHeight, "", "Enter password...", true));
	bool writeBlock = false;
	bool caps = false;
	bool tabbed = false;

	//Start game loop
	while (!quit)
	{
		//Draw screen
		currentScreen = SCREEN_LOGIN;
		draw();

		//Handle incomming events
		while (SDL_PollEvent(&event))
		{
			//Mouse events for the buttons
			for (int i = 0; i < buttonsLogin.size(); i++)
			{
				//Check if the mouse are hovering over any buttons
				buttonsLogin[i]->isMouseOver(event);

				//Check if it clicked it
				string hit = buttonsLogin[i]->onMouseClick(event);
				if (hit == "Quit")
				{
					//Navigate to account creation
					cout << "Bye bye\n";
					quit = true;
				}
				else if (hit == "Login")
				{
//Check server state
#ifdef offline 
	mainMenu();
#endif

#ifdef online 
	//Connect to server
	server = new Network(PORT,serverName);
	server->checkServer();

	if(server->isServerOnlineMethod())
	{
		string u = queryLogin[0]->getContent();
		string p = queryLogin[1]->getContent();
 	
		//Check if login is valid
		if (validateLogin(u, p))
		{
			cout << "Go to menu...\n";
			mainMenu();
		}
		else
		{
			server->send("q");
			delete server;
			cout << "Wrong login!\n";
			queryLogin[0]->clearContent();
			queryLogin[1]->clearContent();				
		}
	}
	else
	{
		cout << "Server Not Online ! \n";
	}
#endif
				}
			}
			
			//For all type-fields
			for (int i = 0; i < queryLogin.size(); i++)
			{
				queryLogin[i]->onMouseClick(event);

				//Figure out what the key does
				SDL_Keycode key = event.key.keysym.sym;

				//If pressed key and this field is selected
				if (event.key.state == SDL_PRESSED && !writeBlock && queryLogin[i]->isWriting())
				{
					if (key == SDLK_BACKSPACE)
					{
						queryLogin[i]->backspace();
						writeBlock = true;
					}
					else if (key == SDLK_LSHIFT)
					{
						caps = true;
					}
					else if (key == SDLK_TAB && !tabbed)
					{
						tabbed = true;
						queryLogin[i]->setActive(false);
						int next = (i + 1) % queryLogin.size();
						queryLogin[next]->setActive(true);
						writeBlock = true;
					}
					else if (key == SDLK_RETURN)
					{
						queryLogin[i]->setActive(false);
					}
					else
					{
						char c = ((caps) ? char(key - 32) : char(key));
						queryLogin[i]->addChar(c);
						writeBlock = true; //To prevent this from running more than one time
					}
				}
				else if (event.key.state == SDL_RELEASED)
				{
					if (key == SDLK_LSHIFT)
					{
						caps = false;
					}
					if (key == SDLK_TAB)
					{
						tabbed = false;
					}
					writeBlock = false;
				}
			}
		}
	}

	//Clear interface
	for (int i = 0; i < buttonsLogin.size(); i++) {
		delete buttonsLogin[i];
	}
	buttonsLogin.clear();

	for (int i = 0; i < queryLogin.size(); i++) {
		delete queryLogin[i];
	}
	queryLogin.clear();
}

void Window::mainMenu()
{
	bool done = false;
	SDL_GetWindowSize(win, &winW, &winH);

	//Create texture from image, check for errors
	string bgStr = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set up buttons
	buttonsMainMenu.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "Build Ship", btnWidth, btnHeight));
	buttonsMainMenu.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Green.png", btnX, btnY + ((btnHeight + offsetY) * 1), "Battle", btnWidth, btnHeight));
	buttonsMainMenu.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + ((btnHeight + offsetY) * 2), "Settings", btnWidth, btnHeight));
	buttonsMainMenu.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + ((btnHeight + offsetY) * 3), "Logout", btnWidth, btnHeight));
    buttonsMainMenu.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Green.png", btnX, btnY + ((btnHeight + offsetY) * 4), "Queue", btnWidth, btnHeight));

	//Create default ship for testing
	string strPlayerShip = "Ship"
	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1")
	+string(" -1 -1")	+string(" -1 -1")	+string(" 0 0")		+string(" -1 -1")	+string(" -1 -1")
	+string(" -1 -1")	+string(" 0 0")		+string(" 0 1")		+string(" 0 0")	+string(" -1 -1")
	+string(" -1 -1")	+string(" -1 -1")	+string(" 0 0")		+string(" -1 -1")	+string(" -1 -1")
	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1")	+string(" -1 -1");

	//playerShip = new Space_Ship(ren, strPlayerShip);
	enemyShip = new Space_Ship(ren, strPlayerShip);
	//Done creating default ship

	//Try to load your ship from server
#ifdef online
	string shipStr = server->getShip();
	if (shipStr.length() > 5) {
		cout << shipStr << endl;
		playerShip = new Space_Ship(ren, shipStr);
	}

	buttonsMainMenu[4]->setStyle(DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png");
#endif

	//Check if has ship
	if (playerShip != NULL)
	{
		buttonsMainMenu[1]->setStyle(DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png");
	}

	//Check if any adjustments were made
	for (int i = 0; i < buttonsMainMenu.size(); i++)
	{
		buttonsMainMenu[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
		//cout << "setPosition(" << scaleX << ", " << scaleY << ")\n";
	}

	//Start game loop
	while (!done)
	{
		//Draw screen
		currentScreen = SCREEN_MAIN_MENU;
		draw();

		//Handle incomming events
		while (SDL_PollEvent(&event))
		{
			//Mouse events for the buttons
			for (int i = 0; i < buttonsMainMenu.size(); i++)
			{
				//Check if the mouse are hovering over any buttons
				buttonsMainMenu[i]->isMouseOver(event);

				//Check if it clicked it
				string hit = buttonsMainMenu[i]->onMouseClick(event);
				if (hit == "Build Ship")
				{
					//Navigate to build space ship
					cout << "Go to editor...\n";
					build();
					buttonsMainMenu[1]->setStyle(DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png");
				}
				else if (hit == "Battle")
				{
					//If you have a valid ship, go to battle
					if (playerShip != NULL)
					{
						cout << "Go to battle...\n";
						battle();
					}
					else {
						cout << "You have not built a ship yet!\n";
					}
				}
                else if (hit == "Queue"){
//Check server state
#ifdef online
	//Join matchmaking
	server->send("m");
	inQue = true;
	queue();
#endif
				}
				else if (hit == "Settings")
				{
					//Go to settings
					cout << "Go to settings...\n";
					settings();

					//Check if any adjustments were made
					for (int i = 0; i < buttonsMainMenu.size(); i++)
					{
						buttonsMainMenu[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
						cout << "setPosition(" << scaleX << ", " << scaleY << ")\n";
					}
				}
				else if (hit == "Logout")
				{
					//Go to login screen
#ifdef online
	server->send("q");
 	delete server;
#endif
					cout << "Go to login screen...\n";
					done = true;
				}
			}
		}
	}

	//Clear interface
	for (int i = 0; i < buttonsMainMenu.size(); i++) {
		delete buttonsMainMenu[i];
	}
	buttonsMainMenu.clear();
}

void Window::queue(){
	buttonsQueue.push_back(new Button(ren, DIR_BUTTONS + "Red.png", DIR_FONTS + "Custom_White.png", btnX, btnY + ((btnHeight + offsetY) * 4), "Leave queue", btnWidth, btnHeight));

	bool done = false;
	while (!done) {
		//Draw screen
		currentScreen = SCREEN_QUEUE;
		draw();

		//Look for match (other player)
		if (server->matchFound()) {
			bool yourTurn = server->starting();
			
			//Load player 1 ship
			string ship = server->getShip();
			if (ship.length() > 5) {
				cout << ship << endl;
				playerShip = new Space_Ship(ren, ship);
			}

			//Load player 2 ship
			string enemyship = server->getEnemyShip();
			if (enemyship.length() > 5) {
				cout << enemyship << endl;
				enemyShip = new Space_Ship(ren, enemyship);
			}

			//Start pvp combat
			Combat *combat = new Combat(playerShip, enemyShip, yourTurn, ren, win, server);

			//Leave queue
			cout << "Return";
			inQue = false;
			done = true;
		}

		//Handle incomming events
		while (SDL_PollEvent(&event)) {
			//Mouse events for the buttons
			for (int i = 0; i < buttonsQueue.size(); i++)
			{
				//Check if the mouse are hovering over any buttons
				buttonsQueue[i]->isMouseOver(event);
			
				//Check if it clicked it
				string hit = buttonsQueue[i]->onMouseClick(event);
				if (hit == "Leave queue")
					{
						//Leave queue
						cout << "Return";
						server->send("m");
						inQue = false;
						done = true;
					}
				}

			}
		}

	//Clear interface
	for (int i = 0; i < buttonsQueue.size(); i++) {
		delete buttonsQueue[i];
	}
	buttonsQueue.clear();
}

void Window::build()
{	
	//Swap background
	string bgStr = DIR_BACKGROUNDS + "Space.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Reset ships
	delete playerShip;
	delete enemyShip;

	//Create new ships
	playerShip = new Space_Ship(ren, background, "Build your ship...");
	enemyShip = new Space_Ship(ren, background, "Build the enemy ship...");

#ifdef online
	//Store the playership on account
	server->saveShip(playerShip->getShipStructure());
#endif

	//Change background back
	string bgStr1 = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr1.c_str());
}

void Window::battle()
{
	//Start the combat
	Combat *combat = new Combat(playerShip, enemyShip, true, ren, win, NULL);
}

void Window::settings()
{
	bool done = false;
	int btnBackY = RESOLUTION_HEIGHT - btnHeight - offsetY;

	//Create the buttons
	buttonsSettings.push_back(new Button(ren, DIR_BUTTONS + "Cyan.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "  Toggle\nFullscreen", btnWidth, btnHeight));
	buttonsSettings.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnBackY, "Back", btnWidth, btnHeight));

	//Check resolution
	for (int i = 0; i < buttonsSettings.size(); i++)
	{
		buttonsSettings[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
	}

	//Start game loop
	while (!done)
	{
		//Draw screen
		currentScreen = SCREEN_SETTINGS;
		draw();

		//Handle incomming events
		while (SDL_PollEvent(&event))
		{
			//Mouse events for the buttons
			for (int i = 0; i < buttonsSettings.size(); i++)
			{
				//Check if the mouse are hovering over any buttons
				buttonsSettings[i]->isMouseOver(event);

				//Check if it clicked it
				string hit = buttonsSettings[i]->onMouseClick(event);
				if (hit == "  Toggle\nFullscreen")
				{
					//Change fullscreen setting
					cout << "Toggle fullscreen...\n";
					isFullscreen = !isFullscreen;
					if (isFullscreen)
					{
						//Set fullscreen on
						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
						SDL_SetWindowSize(win, MAX_RESOLUTION_WIDTH, MAX_RESOLUTION_HEIGHT);
					}
					else
					{
						//Set fullscreen off
						SDL_SetWindowFullscreen(win, SDL_WINDOW_MAXIMIZED);
						SDL_SetWindowSize(win, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
					}

					//Calculate changes
					SDL_GetWindowSize(win, &winW, &winH);
					float scaled = winW / RESOLUTION_WIDTH;
					scaleX = (winW / 2) - (btnWidth / 2);
					scaleY = (winH / 3) - (btnHeight / 2);

					//Update positions
					for (int i = 0; i < buttonsSettings.size(); i++)
					{
						buttonsSettings[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
					}
				}
				else if (hit == "Back")
				{
					//Return to main menu
					cout << "Go to back...\n";
					done = true;
				}
			}
		}
	}

	//Clear interface
	for (int i = 0; i < buttonsSettings.size(); i++) {
		delete buttonsSettings[i];
	}
	buttonsSettings.clear();
}

bool Window::validateLogin(string user, string code)
{	
	//Print data
	cout << "Username: " << user << "\nPassword: " << code << endl;

	//Format message
	   string send = "l-"+ user+"/"+code;

	//Send data message
	server->send((char * )send.c_str());
	
	//Check if username exists
	return server->checkLogin();
}

void Window::draw()
{
	//Reset screen
	SDL_RenderClear(ren);

	//Draw background
	SDL_RenderCopy(ren, background, NULL, NULL);

	if (currentScreen == SCREEN_LOGIN)
	{
		//Draw buttons
		for (int i = 0; i < buttonsLogin.size(); i++)
		{
			buttonsLogin[i]->draw();
		}

		//Draw queryLogin
		for (int i = 0; i < queryLogin.size(); i++)
		{
			queryLogin[i]->draw();
		}
	}
	else if (currentScreen == SCREEN_MAIN_MENU)
	{
		//Draw buttons
		for (int i = 0; i < buttonsMainMenu.size(); i++)
		{
			buttonsMainMenu[i]->draw();
		}
	}
	else if (currentScreen == SCREEN_SETTINGS)
	{
		//Draw buttons
		for (int i = 0; i < buttonsSettings.size(); i++)
		{
			buttonsSettings[i]->draw();
		}
	}
	else if (currentScreen == SCREEN_QUEUE)
	{
		//Draw buttons
		for (int i = 0; i < buttonsQueue.size(); i++)
		{
			buttonsQueue[i]->draw();
		}
	}

	//Render screen
	SDL_RenderPresent(ren);
}