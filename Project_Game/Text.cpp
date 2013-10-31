#include "Text.h"



Text::Text()
{
	//Not used
}

Text::~Text()
{
}

Text::Text(SDL_Renderer *ren, string txt, string fnt, string bg)
{
	renCopy = ren;

	//Set text string
	text = txt;

	//Set character size
	charSize = new SDL_Rect();
	charSize->w = 16;
	charSize->h = 16;
	charSize->x = 0;
	charSize->y = 0;

	//Set base position
	positionX = 0;
	positionY = 0;

	//Check if font is specified
	if (fnt.compare("") != 0)
	{
		//Load specified font
		font = IMG_LoadTexture(ren, fnt.c_str());
	}
	else
	{
		//Load default font
		string default_font = DIR_FONTS + "Custom_Orange.png";
		font = IMG_LoadTexture(ren, default_font.c_str());
	}

	//Check if background is specified
	if (bg.compare("") != 0)
	{
		//Load specified background
		background = IMG_LoadTexture(ren, bg.c_str());
	}
	else
	{
		//Load default background
		string default_background = DIR_FONTS + "FontBG_White.png";
		background = IMG_LoadTexture(ren, default_background.c_str());
	}

	//Create working positions
	SDL_Rect *tmpSrc = new SDL_Rect();
	tmpSrc->w = charSize->w - 1;
	tmpSrc->h = charSize->h - 1;
	tmpSrc->x = 0;
	tmpSrc->y = 0;

	SDL_Rect *tmpDst = new SDL_Rect();
	tmpDst->w = charSize->w - 1;
	tmpDst->h = charSize->h - 1;
	tmpDst->x = 0;
	tmpDst->y = 0;

	int curLine = 0;
	int curRow = 0;
	width = 0;

	cout << "Created text...\n" << '"' << endl;

	//For all characters in the text
	for (char &letter : text)
	{
		//Check if the line expanded
		if (curLine + 1 > width)
		{
			width = curLine + 1;
		}

		//Check letter
		if (letter == '\n' || (letter == ' ' && curLine >= LINE_LEN))
		{
			//Create new line
			tmpDst->y = ++curRow * tmpDst->h;
			curLine = 0;

			//Print new line
			cout << endl;
		}
		else
		{
			//Get letter index
			int i = 0;

			if (letter == ' ')
				i = FONT_SPACE;

			else if (letter >= 48 && letter <= 57) //0-9
				i = FONT_0 + (letter - 48);

			else if (letter >= 65 && letter <= 90) //A-Z
				i = FONT_A + (letter - 65);
			
			else if (letter >= 97 && letter <= 122) //a-z
				i = FONT_a + (letter - 97);
			
			else if (letter == '+')
				i = FONT_PLUSS;

			else if (letter == '-')
				i = FONT_MINUS;

			else if (letter == '(')
				i = FONT_OPENBRACKET;

			else if (letter == ')')
				i = FONT_CLOSEBRACKET;

			else if (letter == '=')
				i = FONT_EQUALS;

			else if (letter == '!')
				i = FONT_EXCLAMATION;

			else if (letter == '?')
				i = FONT_QUESTION;

			else if (letter == '%')
				i = FONT_PERCENT;

			else if (letter == '/')
				i = FONT_SLASH;

			else if (letter == '*')
				i = FONT_STAR;

			else if (letter == '.')
				i = FONT_PERIOD;

			else if (letter == ',')
				i = FONT_COMMA;

			else if (letter == ':')
				i = FONT_COLON;

			else if (letter == 39)
				i = FONT_APOSTROPHE;
			

			int posX = i % FONT_COLUMNS;
			int posY = i / FONT_COLUMNS;

			//Set source position
			tmpSrc->x = posX * charSize->w + 1;
			tmpSrc->y = posY * charSize->h + 1;

			//Make source copy
			SDL_Rect *cpySrc = new SDL_Rect();
			cpySrc->w = tmpSrc->w;
			cpySrc->h = tmpSrc->h;
			cpySrc->x = tmpSrc->x;
			cpySrc->y = tmpSrc->y;

			//Cheaty way to make sure commas get marked to be flipped later
			if (letter == ',')
				cpySrc->w -= 1;

			//Save source position
			srcRects.push_back(cpySrc);

			//Set destionation position
			tmpDst->x = curLine++ * (tmpDst->w - OFFSET);

			//Make destionation copy
			SDL_Rect *cpyDst = new SDL_Rect();
			cpyDst->w = tmpDst->w;
			cpyDst->h = tmpDst->h;
			cpyDst->x = tmpDst->x;
			cpyDst->y = tmpDst->y;

			//Save destionation position
			dstRects.push_back(cpyDst);

			//Print letter
			cout << letter;
		}
	}

	rows = curRow + 1;
	currentScale = 1;
	cout << endl << '"' << endl;
}

void Text::draw(SDL_Renderer *ren)
{
	//Create background position
	SDL_Rect *tmpSrc = new SDL_Rect();
	tmpSrc->w = 15;
	tmpSrc->h = 15;
	tmpSrc->x = 1;
	tmpSrc->y = 1;

	SDL_Rect *tmpDst = new SDL_Rect();
	tmpDst->w = 15;
	tmpDst->h = 15;
	tmpDst->x = 0;
	tmpDst->y = 0;

	//For all letters for the entire with
	for (int y = 0; y < rows; y++)
	{
		//Set position y
		tmpDst->y = y * tmpDst->h + positionY;
		for (int x = 0; x < width; x++)
		{
			//Set position x
			tmpDst->x = x * (tmpDst->w - OFFSET) + positionX;

			//Draw background
			SDL_RenderCopy(ren, background, tmpSrc, tmpDst);
		}
	}

	//For all letters in the vector
	for (int i = 0; i < dstRects.size(); i++)
	{
		//Draw letter
		if (srcRects[i]->w == 14)
		{
			if (currentScale != 1)
			{
				SDL_RenderCopyEx(ren, font, srcRects[i], dstScaled[i], NULL, NULL, SDL_FLIP_HORIZONTAL);
			}
			else {
				//Flip comma to be the right way
				SDL_RenderCopyEx(ren, font, srcRects[i], dstRects[i], NULL, NULL, SDL_FLIP_HORIZONTAL);
			}
		}
		else
		{
			if (currentScale != 1)
			{
				SDL_RenderCopy(ren, font, srcRects[i], dstScaled[i]);
			}
			else {
				//Draw normal
				SDL_RenderCopy(ren, font, srcRects[i], dstRects[i]);
			}
		}
	}
}

void Text::setText(string txt)
{
	srcRects.clear();
	dstRects.clear();
	dstScaled.clear();

	//Set text string
	text = txt;

	//Create working positions
	SDL_Rect *tmpSrc = new SDL_Rect();
	tmpSrc->w = charSize->w - 1;
	tmpSrc->h = charSize->h - 1;
	tmpSrc->x = 0;
	tmpSrc->y = 0;

	SDL_Rect *tmpDst = new SDL_Rect();
	tmpDst->w = charSize->w - 1;
	tmpDst->h = charSize->h - 1;
	tmpDst->x = 0;
	tmpDst->y = positionY;

	int curLine = 0;
	int curRow = 0;
	width = 0;

	cout << "Updated text...\n" << '"' << endl;

	//For all characters in the text
	for (char &letter : text)
	{
		//Check if the line expanded
		if (curLine + 1 > width)
		{
			width = curLine + 1;
		}

		//Check letter
		if (letter == '\n' || (letter == ' ' && curLine >= LINE_LEN))
		{
			//Create new line
			tmpDst->y = ++curRow * tmpDst->h;
			curLine = 0;

			//Print new line
			cout << endl;
		}
		else
		{
			//Get letter index
			int i = 0;

			if (letter == ' ')
				i = FONT_SPACE;

			else if (letter >= 48 && letter <= 57) //0-9
				i = FONT_0 + (letter - 48);

			else if (letter >= 65 && letter <= 90) //A-Z
				i = FONT_A + (letter - 65);
			
			else if (letter >= 97 && letter <= 122) //a-z
				i = FONT_a + (letter - 97);
			
			else if (letter == '+')
				i = FONT_PLUSS;

			else if (letter == '-')
				i = FONT_MINUS;

			else if (letter == '(')
				i = FONT_OPENBRACKET;

			else if (letter == ')')
				i = FONT_CLOSEBRACKET;

			else if (letter == '=')
				i = FONT_EQUALS;

			else if (letter == '!')
				i = FONT_EXCLAMATION;

			else if (letter == '?')
				i = FONT_QUESTION;

			else if (letter == '%')
				i = FONT_PERCENT;

			else if (letter == '/')
				i = FONT_SLASH;

			else if (letter == '*')
				i = FONT_STAR;

			else if (letter == '.')
				i = FONT_PERIOD;

			else if (letter == ',')
				i = FONT_COMMA;

			else if (letter == ':')
				i = FONT_COLON;

			else if (letter == 39)
				i = FONT_APOSTROPHE;
			

			int posX = i % FONT_COLUMNS;
			int posY = i / FONT_COLUMNS;

			//Set source position
			tmpSrc->x = posX * charSize->w + 1;
			tmpSrc->y = posY * charSize->h + 1;

			//Make source copy
			SDL_Rect *cpySrc = new SDL_Rect();
			cpySrc->w = tmpSrc->w;
			cpySrc->h = tmpSrc->h;
			cpySrc->x = tmpSrc->x;
			cpySrc->y = tmpSrc->y;

			//Cheaty way to make sure commas get marked to be flipped later
			if (letter == ',')
				cpySrc->w -= 1;

			//Save source position
			srcRects.push_back(cpySrc);

			//Set destionation position
			tmpDst->x = curLine++ * (tmpDst->w - OFFSET) + positionX;

			//Make destionation copy
			SDL_Rect *cpyDst = new SDL_Rect();
			cpyDst->w = tmpDst->w;
			cpyDst->h = tmpDst->h;
			cpyDst->x = tmpDst->x;
			cpyDst->y = tmpDst->y;

			//Save destionation position
			dstRects.push_back(cpyDst);

			//Print letter
			cout << letter;
		}
	}

	rows = curRow + 1;
	currentScale = 1;
	cout << endl << '"' << endl;
}

void Text::setPosition(int x, int y)
{
	//Calculate change
	const int changeX = (x - positionX);
	const int changeY = (y - positionY);

	//Update letter positions
	for (int i = 0; i < dstRects.size(); i++)
	{
		dstRects[i]->x += changeX;
		dstRects[i]->y += changeY;
	}

	//Update absolute position
	positionX = x;
	positionY = y;
}

int Text::getWidth()
{
	return (width * (charSize->w - OFFSET));
}

int Text::getHeight()
{
	return (rows * charSize->h);
}

string Text::getText()
{
	return text;
}

void Text::setFont(SDL_Renderer *ren, string newFont)
{
	font = IMG_LoadTexture(ren, newFont.c_str());
}