#pragma once
#include <SDL.h>
#include <iostream>
#include "Constants.h"
#include "Space_Ship.h"
#include "Text.h"
#include "Button.h"
#include "Combat.h"
#include "EditText.h"
#include <SDL_mixer.h>
#include "Network.h"
using namespace std;



class Window
{
private:
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Texture *background;
	Mix_Chunk *music;
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
	Space_Ship *enemyShip;

	//Network
	Network *server;

	//Interface data
	screenType currentScreen;
	vector <Button*> buttonsLogin;
	vector <EditText*> queryLogin;
	vector <Button*> buttonsMainMenu;
	vector <Button*> buttonsSettings;

public:
    Window();
    ~Window();
	
	int getWindowState();
	void login();
	void mainMenu();
	void build();
	void battle();
	void settings();
	bool validateLogin(string user, string code);
	void draw();
};

