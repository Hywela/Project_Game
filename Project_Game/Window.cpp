#include <iostream>

#include "Window.h"
#include "Constants.h"
#include "Space_Ship.h"
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
	bmp = SDL_LoadBMP(img_str);
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
					cout << "Left button\n";
					break;
				}
				
				default:
				{
					cout << button << " (Unhandeled button!)\n";
					break;
				}
			}
		}
	}
}

void Window::runWindow()
{
	//Create player one ship
	Space_Ship *player = new Space_Ship(ren);

	while (!quit)
	{
		//Reset screen
		SDL_RenderClear(ren);

		//Draw background
		SDL_RenderCopy(ren, tex, NULL, NULL);

		//Draw players ship
		player->draw(ren);

		//Render screen
		SDL_RenderPresent(ren);

		//Handle incomming events
		handleEvents();
	}
}