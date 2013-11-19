#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Constants.h"
using namespace std;



class Text
{
private:
	static const int FONT_ROWS = 8;
	static const int FONT_COLUMNS = 32;
	static const int LINE_LEN = 30;
	static const int OFFSET = 6;

	SDL_Texture *font;			//The font image of this text
	SDL_Texture *background;	//Background image for this text
	SDL_Rect *charSize;			//Size of each character
	string text;				//String of the characters
	int rows;					//Number of lines
	int width;					//Number of characters in longest row
	int positionX;				//Position in the window
	int positionY;				//...
	enum charIndex {			//List of all character indexes
		FONT_SPACE,				//This is empty
		FONT_0,					//Start of numbers
		FONT_A = 11,			//Start of upper case
		FONT_a = 37,			//Start of lower case
		FONT_PLUSS = 223,		//Character[ + ]
		FONT_MINUS = 230,		//Character[ - ]
		FONT_OPENBRACKET = 236,	//Character[ ( ]
		FONT_CLOSEBRACKET,		//Character[ ) ]
		FONT_EQUALS = 239,		//Character[ = ]
		FONT_EXCLAMATION,		//Character[ ! ]
		FONT_QUESTION,			//Character[ ? ]
		FONT_PERCENT,			//Character[ % ]
		FONT_SLASH,				//Character[ / ]
		FONT_STAR,				//Character[ * ]
		FONT_COMMA,				//Character[ , ]
		FONT_PERIOD = 247,		//Character[ . ]
		FONT_COLON = 250,		//Character[ : ]
		FONT_APOSTROPHE = 255};	//Character[ ' ]
	vector <SDL_Rect*> srcRects;	//Take this rectangles...
	vector <SDL_Rect*> dstRects;	//...and place them here!
	vector <SDL_Rect*> dstScaled;	//...scaled to this
	float currentScale;
	SDL_Renderer *ren;

public:
	Text();
	~Text();
	Text(SDL_Renderer *rend, string txt = "", string fnt = "", string bg = "");

	void draw();
	void setText(string txt);
	void setPosition(int x, int y);
	int getWidth();
	int getHeight();
	string getText();
	void setFont(string newFont);
};