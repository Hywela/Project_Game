#include "Window.h"



Window::Window()
{
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
	server = new Network();
}

Window::~Window()
{
	delete server;
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

	vector <Button*> buttons;
	vector <EditText*> queryLogin;

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
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX - (btnWidth / 2) - 10, btnY * 3, "Create account", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX + (btnWidth / 2) + 10, btnY * 3, "Login", btnWidth, btnHeight));

	queryLogin.push_back(new EditText(ren, DIR_EDITTEXTS + "White.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, btnWidth, btnHeight, "", "Enter username..."));
	queryLogin.push_back(new EditText(ren, DIR_EDITTEXTS + "White.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + btnHeight + 20, btnWidth, btnHeight, "", "Enter password...", true));
	bool writeBlock = false;
	bool caps = false;
	bool tabbed = false;

	//Start game loop
	while (!quit)
	{
		//Reset screen
		SDL_RenderClear(ren);

		//Draw background
		SDL_RenderCopy(ren, background, NULL, NULL);

		//Draw buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->draw(ren);
		}

		//Draw queryLogin
		for (int i = 0; i < queryLogin.size(); i++)
		{
			queryLogin[i]->draw(ren);
		}

		//Render screen
		SDL_RenderPresent(ren);

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
				if (hit == "Create account")
				{
					//Navigate to account creation
					cout << "Go to new account...\n";
					//crateAccount();
				}
				else if (hit == "Login")
				{
					string u = queryLogin[0]->getContent();
					string p = queryLogin[1]->getContent();
					if (validateLogin(u, p))
					{
						cout << "Go to menu...\n";
						mainMenu();
					}
					else
					{
						cout << "Wrong login!\n";
						queryLogin[0]->clearContent();
						queryLogin[1]->clearContent();				
					}
				}
			}

			for (int i = 0; i < queryLogin.size(); i++)
			{
				queryLogin[i]->onMouseClick(event);

				//Figure out what the key does
				SDL_Keycode key = event.key.keysym.sym;

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
						writeBlock = true;
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
}

void Window::mainMenu()
{
	bool done = false;
	vector <Button*> buttons;
	SDL_GetWindowSize(win, &winW, &winH);

	//Create texture from image, check for errors
	string bgStr = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set up buttons
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "Build Ship", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Green.png", btnX, btnY + ((btnHeight + offsetY) * 1), "Battle", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + ((btnHeight + offsetY) * 2), "Settings", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + ((btnHeight + offsetY) * 3), "Logout", btnWidth, btnHeight));

	if (playerShip != NULL)
	{
		buttons[1]->setStyle(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png");
	}

	//Check if any adjustments were made
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
		cout << "setPosition(" << scaleX << ", " << scaleY << ")\n";
	}

	//Start game loop
	while (!done)
	{
		//Reset screen
		SDL_RenderClear(ren);

		//Draw background
		SDL_RenderCopy(ren, background, NULL, NULL);

		//Draw buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->draw(ren);
		}

		//Render screen
		SDL_RenderPresent(ren);

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
				if (hit == "Build Ship")
				{
					//Navigate to build space ship
					cout << "Go to editor...\n";
					build();
					buttons[1]->setStyle(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png");
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
				else if (hit == "Settings")
				{
					//Go to settings
					cout << "Go to settings...\n";
					settings();

					//Check if any adjustments were made
					for (int i = 0; i < buttons.size(); i++)
					{
						buttons[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
						cout << "setPosition(" << scaleX << ", " << scaleY << ")\n";
					}
				}
				else if (hit == "Logout")
				{
					//Go to settings
					cout << "Go to login screen...\n";
					done = true;
				}
			}
		}
	}
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
	playerShip = new Space_Ship(ren, background);
	enemyShip = new Space_Ship(ren, background);

	//Change background back
	string bgStr1 = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr1.c_str());
}

void Window::battle()
{
	//Start the combat
	Combat *combat = new Combat(playerShip, enemyShip, true, ren, win);
}

void Window::settings()
{
	bool done = false;
	vector <Button*> buttons;
	int btnBackY = RESOLUTION_HEIGHT - btnHeight - offsetY;

	//Create the buttons
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Cyan.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "  Toggle\nFullscreen", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnBackY, "Back", btnWidth, btnHeight));

	//Check resolution
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
	}

	//Start game loop
	while (!done)
	{
		//Reset screen
		SDL_RenderClear(ren);

		//Draw background
		SDL_RenderCopy(ren, background, NULL, NULL);

		//Draw buttons
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i]->draw(ren);
		}

		//Render screen
		SDL_RenderPresent(ren);

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
					for (int i = 0; i < buttons.size(); i++)
					{
						buttons[i]->setPosition(scaleX, scaleY + ((btnHeight + offsetY) * i));
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
}

bool Window::validateLogin(string user, string code)
{
	//Print data
	cout << "Username: " << user << "\nPassword: " << code << endl;

	//Format message
	string send = "-u "+ user;

	//Send command message
	server->handler_send((char * )"-u");

	//Send data message
	server->handler_send((char * )user.c_str());
	
	//Check if username exists
	return true; //server->handler_check_login();
}