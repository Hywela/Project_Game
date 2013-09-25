#include <iostream>

#include "Window.h"
#include "Constants.h"
#include "Module_Gun.h"
using namespace std;



Window::Window()
{
	//Starting SDL, initializing main
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		window_state = 1;
	}

	//Create window, check for errors
    win = SDL_CreateWindow(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
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


	//Load image, check for errors
	string img_dir = DIR_BACKGROUNDS + "Space.bmp";
	const char *img_str = img_dir.c_str();
	bmp = IMG_Load(img_str);

	if (bmp == nullptr)
	{
		cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		window_state = 4;
	}

	//Create texture from image, check for errors
	tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr)
	{
		cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
		window_state = 5;
	}

	//Set start state
	quit = false;
	isFullscreen = false;
}

Window::~Window()
{
	//Close program
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

int Window::getWindowState()
{
	return window_state;
}

void Window::handleEvents()
{
	//Check for new events
	while (SDL_PollEvent(&event))
	{
		//Check if a key was PRESSED
		if (event.key.state == SDL_PRESSED)
		{
			//Figure out what the key does
			SDL_Keycode key = event.key.keysym.sym;
			
			cout << "[KEY-EVENT]: ";
			switch (key)
			{
				case SDLK_ESCAPE:
				{
					cout << "Escape";
					quit = true;
					break;
				}
				case SDLK_TAB:
				{
					cout << "Tab";
					isFullscreen = !isFullscreen;
					if (isFullscreen)
					{

						SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);
						SDL_SetWindowSize(win, MAX_RESOLUTION_WIDTH, MAX_RESOLUTION_HEIGHT);
					}
					else
					{
						SDL_SetWindowFullscreen(win, SDL_WINDOW_MAXIMIZED);
						SDL_SetWindowSize(win, RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
					}
					
					break;
				}
				
				default:
				{
					cout << (char)key << " (Unhandeled key!)";
					break;
				}
			}
			cout << endl;
		}
		else if (event.button.state == SDL_PRESSED)
		{
			//Figure out what the key does
			SDL_Keycode button = event.button.button;
			
			cout << "[MOUSE-EVENT]: ";
			switch (button)
			{
				case SDL_BUTTON_LEFT:
				{
					cout << "Left button";
					break;
				}
				
				default:
				{
					cout << button << " (Unhandeled button!)";
					break;
				}
			}
			cout << endl;
		}
	}
}

void Window::runWindow()
{
	//Create space ship
	const int SHIP_WIDTH = 3;
	const int SHIP_HEIGHT = 3;
	Module *ship[SHIP_HEIGHT][SHIP_WIDTH];

	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			ship[y][x] = new Module(ren, DIR_MODULES + "Empty.png", 0, 0, 0);
		}
	}

	//Swap center module for testing
	delete ship[1][1];
	ship[1][1] = new Module_Gun(ren, DIR_MODULES + "Turret.png", 1, 10, 75, 2, 5, 0);

	//Draw the space ship
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//ship[y][x]->draw(ren);
			cout << "[" << ship[y][x]->getCurrentHealth() << "/" << ship[y][x]->getMaxHealth() << "]\t";
		}
		cout << "\n\n";
	}
    ship[1][1]->setPostion(20,20);
    ship[1][1]->setSize(20,20);
	while (!quit)
	{
		//Reset screen
		SDL_RenderClear(ren);
       
		//Draw background
		SDL_RenderCopy( ren, tex, NULL, NULL);
        ship[1][1]->draw(ren);
		//Draw other stuff
		//...
 

		//Render screen
		SDL_RenderPresent(ren);

		//Handle incomming events
		handleEvents();
	}
}