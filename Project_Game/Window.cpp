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

	//Set start state
	quit = false;
	isFullscreen = false;
	playerShip = NULL;
	server = new Network();
}

Window::~Window()
{
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

void Window::mainMenu()
{

	vector <Button*> buttons;
	SDL_GetWindowSize(win, &winW, &winH);

	//Create texture from image, check for errors
	string bgStr = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set up button properties
	btnWidth = 200;
	btnHeight = 40;
	startY = 220;
	offsetY = 20;
	btnX = (winW / 2) - (btnWidth / 2);
	btnY = startY - (((btnHeight + 10) * 4) / 2);
	scaleX = btnX;
	scaleY = btnY;

	//Set up buttons
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY, "Build Ship", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Green.png", btnX, btnY + ((btnHeight + offsetY) * 1), "Battle", btnWidth, btnHeight));
	buttons.push_back(new Button(ren, DIR_BUTTONS + "Golden.png", DIR_FONTS + "Custom_Orange.png", btnX, btnY + ((btnHeight + offsetY) * 2), "Settings", btnWidth, btnHeight));

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
			}
		}
	}
}

void Window::build()
{
	//Swap background
	string bgStr = DIR_BACKGROUNDS + "Space.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Reset player ship
	delete playerShip;

	//Create new player ship
	playerShip = new Space_Ship(ren, background);

	//Change background back
	string bgStr1 = DIR_BACKGROUNDS + "Main_Menu.png";
	background = IMG_LoadTexture(ren, bgStr1.c_str());
}

void Window::battle()
{
	//Generate a ship like yours to fight (for now...)
	Space_Ship *generatedEnemy = new Space_Ship(*playerShip);

	//Start the combat
	Combat *combat = new Combat(*playerShip, *generatedEnemy, true);
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
					scaleY = (startY * scaled) - (((btnHeight + 10) * 4) / 2);

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