#include "Window.h"
#include <iostream>
#include "Module_Gun.h"
#include "SDL.h"
using namespace std;


Module modules[3][3];

 
int main(int argc, char** argv) {
	//Starting SDL, initializing main
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	//Create window, check for errors
    SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		return 1;
	}

	//Create renderer, check for errors
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		return 1;
	}

	//Load image, check for errors
	SDL_Surface *bmp = SDL_LoadBMP("hello.bmp");
	if (bmp == nullptr)
	{
		cout << "SDL_LoadBMP Error: " << SDL_GetError() << endl;
		return 1;
	}

	//Create texture from image, check for errors
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	if (tex == nullptr)
	{
		cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl;
		return 1;
	}

	//Update the screen (renderer)
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, tex, NULL, NULL);
	SDL_RenderPresent(ren);

	//Fill module-array with test modules
	int i = 1;
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++) 
		{
			modules[y][x] = Module_Gun("", 0, i++, 75, 2, 5, 0);
		}
	}

	cout << endl;

	//Do some damage tests
	modules[1][1].onHit(8);
	modules[2][2].onHit(5);
	modules[2][0].onHit(3);

	cout << endl;

	//Display the module-array after changes
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			cout << "[" << modules[y][x].getCurrentHealth() << " / " << modules[y][x].getMaxHealth() << "]";
		}
		cout << endl;
	}
	system("pause");

	//Sleep in 2000 frames or milliseconds (not sure)
	//SDL_Delay(2000);

	//Close program
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
 
	return 0;
}