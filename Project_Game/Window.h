#pragma once
#include <SDL.h>
#include <SDL_image.h>


class Window
{
private:
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Surface *bmp;
	SDL_Texture *tex;
	int window_state;
	bool isFullscreen;
	bool quit;
	SDL_Event event;

public:
    Window();
    ~Window();
	
	int getWindowState();
	void handleEvents();
	void runWindow();
};

