#pragma once
#include "Text.h"



class EditText
{
private:
	SDL_Renderer *ren;
	SDL_Texture *frame;
	SDL_Texture *frameHighlight;
	SDL_Rect *srcRect;
	SDL_Rect *dstRect;
	bool isActive;
	string content;
	Text *text;
	Text *tooltip;
	Text *objective;
	int centreY;
	bool sensitiveText;

public:
	EditText();
	EditText(SDL_Renderer *rend, string img, string font, int x, int y, int w, int h, string obj = "", string tip = "", bool hide = false);
	~EditText();

	void addChar(char c);
	void backspace();
	void setPosition(int x, int y);
	bool isMouseOver(SDL_Event event);
	void onMouseClick(SDL_Event event);
	void draw();
	bool isWriting();
	void setActive(bool state);
	string getContent();
	void clearContent();
};