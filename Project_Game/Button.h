#pragma once
#include "Text.h"
#include <SDL_mixer.h>



class Button
{
private:
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
	Button(SDL_Renderer *ren, string img, string font, int x, int y, string tip = "", int w = -1, int h = -1);
	~Button();

	void setPosition(int x, int y);
	void scale(float ratio);
	bool isMouseOver(SDL_Event event);
	string onMouseClick(SDL_Event event);
	void draw(SDL_Renderer *ren);
	void setStyle(SDL_Renderer *ren, string newStyle, string newFont = "");
	void setText(string txt);
};