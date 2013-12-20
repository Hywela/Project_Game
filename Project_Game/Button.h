#pragma once
#include "Text.h"
#include <SDL_mixer.h>



class Button
{
private:
	SDL_Renderer *ren;
	SDL_Texture *style;
	SDL_Texture *styleHighlight;
	SDL_Rect *srcRect;
	SDL_Rect *dstRect;
	Text *tooltip;
	bool highlight;
	int buttonCentreX;
	int buttonCentreY;
	Mix_Chunk *soundClick;

	float currentScale;

public:
	Button();
	Button(SDL_Renderer *rend, string img, string font, int x, int y, string tip = "", int w = -1, int h = -1);
	~Button();

	void setPosition(int x, int y);

	/* Moves the button a distance calculated from scale */
	void scale(float ratio);

	bool isMouseOver(SDL_Event event);
	string onMouseClick(SDL_Event event);
	void draw();
	void setStyle(string newStyle, string newFont = "");
	void setText(string txt);
};