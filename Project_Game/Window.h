#pragma once
#include <SDL.h>
#include <iostream>
#include "Constants.h"
#include "Space_Ship.h"
#include "Text.h"
#include "Button.h"
#include "Combat.h"
#include "Network.h"
using namespace std;



class Window
{
private:
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	int win_w;
	int win_h;
	int window_state;
	bool isFullscreen;
	bool quit;
	SDL_Event event;

	//Button properties:
	int winW;
	int winH;
	int btnWidth;
	int btnHeight;
	int startY;
	int offsetY;
	int btnX;
	int btnY;
	int scaleX;
	int scaleY;

	//Ship:
	Space_Ship *playerShip;

	//Network
	Network *server;

public:
    Window();
    ~Window();
	
	int getWindowState();
	void mainMenu();
	void build();
	void battle();
	void settings();
};

