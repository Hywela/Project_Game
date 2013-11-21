#include "Button.h"



Button::Button()
{
}

Button::Button(SDL_Renderer *rend, string img, string font, int x, int y, string tip, int w, int h)
{
	ren = rend;

	//Load background music
	string soundStr = DIR_EFFECTS + "Button Click.wav";
	soundClick = Mix_LoadWAV(soundStr.c_str());

	//Set button style
	string imgHighlight = DIR_BUTTONS + "Highlight.png";
	style = IMG_LoadTexture(ren, img.c_str());
	styleHighlight = IMG_LoadTexture(ren, imgHighlight.c_str());

	//Set tooltip
	tooltip = new Text(ren, tip, font, "Transparent");

	//Set source
	srcRect = new SDL_Rect();
	srcRect->x = 0;
	srcRect->y = 0;
	srcRect->w = 400;
	srcRect->h = 60;

	//Set destination
	int width = tooltip->getWidth();
	int height = tooltip->getHeight();
	dstRect = new SDL_Rect();
	dstRect->x = x;
	dstRect->y = y;
	dstRect->w = ((w == -1) ? width : w);
	dstRect->h = ((h == -1) ? height : h);

	//Set position
	buttonCentreX = (dstRect->w - width) / 2;
	buttonCentreY = (dstRect->h - height) / 2;
	tooltip->setPosition(x + buttonCentreX, y + buttonCentreY);

	//Set highlight
	highlight = false;
	currentScale = 1;
}

Button::~Button()
{
	cout << "A button was destroyed!\n";
}

void Button::setPosition(int x, int y)
{
	//Set new position
	dstRect->x = x;
	dstRect->y = y;

	//Update position of the text
	tooltip->setPosition(x + buttonCentreX, y + buttonCentreY);
}

bool Button::isMouseOver(SDL_Event event)
{
	bool isOver = false;
	highlight = false;
	int mouseX = event.button.x;
	int mouseY = event.button.y;

	//Check if inside x-boundry
	if (mouseX >= dstRect->x && mouseX <= dstRect->x + dstRect->w)
	{
		//Check if inside y-boundry
		if (mouseY >= dstRect->y && mouseY <= dstRect->y + dstRect->h)
		{
			isOver = true;
			highlight = true;
		}
	}

	//Return result
	return isOver;
}

string Button::onMouseClick(SDL_Event event)
{
	string hit = "";
	int mouseX = event.button.x;
	int mouseY = event.button.y;
	
	//Check if this button is pressed
	if (highlight && event.button.state == SDL_PRESSED) {
		hit = tooltip->getText();
		cout << "[BUTTON-CLICK]: " << hit << endl;
		Mix_PlayChannelTimed(1, soundClick, 0, NULL);
	}

	//Return text of the button
	return hit;
}

void Button::draw()
{
	//Draw button
	SDL_RenderCopy(ren, style, srcRect, dstRect);

	//Highlight button if nessesary
	if (highlight)
	{
		SDL_RenderCopy(ren, styleHighlight, srcRect, dstRect);
	}

	//Draw text
	tooltip->draw();
}

void Button::scale(float ratio)
{
	//If already scaled
	if (currentScale != 1)
	{
		//Reset scaling first
		dstRect->x /= currentScale;
		dstRect->y /= currentScale;
	}

	//Set new scale
	dstRect->x *= ratio;
	dstRect->y *= ratio;
	tooltip->setPosition(dstRect->x + buttonCentreX, dstRect->y + buttonCentreY);

	//Save scale
	currentScale = ratio;
}

void Button::setStyle(string newStyle, string newFont)
{
	//Change button style
	style = IMG_LoadTexture(ren, newStyle.c_str());

	//Change text font, if passed
	if (newFont != "")
	{
		tooltip->setFont(newFont);
	}
}

void Button::setText(string txt)
{
	tooltip->setText(txt);
}