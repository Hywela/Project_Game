#include "EditText.h"



EditText::EditText()
{
}

EditText::EditText(SDL_Renderer *ren, string img, string font, int x, int y, int w, int h, string obj, string tip, bool hide)
{
	//Set button style
	string imgHighlight = DIR_EDITTEXTS + "Highlight.png";
	frame = IMG_LoadTexture(ren, img.c_str());
	frameHighlight = IMG_LoadTexture(ren, imgHighlight.c_str());

	//Set tooltip
	text = new Text(ren, "", font, "Transparent");
	tooltip = new Text(ren, tip, font, "Transparent");
	objective = new Text(ren, obj, font, DIR_FONTS + "FontBG_White.png");
	content = "";

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
	centreY = (dstRect->h - height) / 2;
	tooltip->setPosition(x + 10, y + centreY);
	text->setPosition(x + 10, y + centreY);
	objective->setPosition(x + 2, y - objective->getHeight());

	//Set highlight
	isActive = false;
	sensitiveText = hide;
}

EditText::~EditText()
{
}

void EditText::addChar(char c)
{
	content += c;
	string newText = content;

	if (sensitiveText)
	{
		newText = "";
		for (int i = 0; i < content.size(); i++)
		{
			newText += "*";
		}
	}

	text->setText(newText);
	cout << "EditText: " << content << endl;
}

void EditText::backspace()
{
	if (content != "")
	{
		content.pop_back();
		string newText = content;

		if (sensitiveText)
		{
			newText = "";
			for (int i = 0; i < content.size(); i++)
			{
				newText += "*";
			}
		}

		text->setText(newText);
	}
	cout << "EditText: " << content << endl;
}

void EditText::setPosition(int x, int y)
{
	//Set new position
	dstRect->x = x;
	dstRect->y = y;

	//Update position of the text
	tooltip->setPosition(x + 10, y + centreY);
	text->setPosition(x + 10, y + centreY);
}

bool EditText::isMouseOver(SDL_Event event)
{
	bool isOver = false;
	int mouseX = event.button.x;
	int mouseY = event.button.y;

	//Check if inside x-boundry
	if (mouseX >= dstRect->x && mouseX <= dstRect->x + dstRect->w)
	{
		//Check if inside y-boundry
		if (mouseY >= dstRect->y && mouseY <= dstRect->y + dstRect->h)
		{
			isOver = true;
		}
	}

	//Return result
	return isOver;
}

void EditText::onMouseClick(SDL_Event event)
{
	//Check if this button is pressed
	if (event.button.state == SDL_PRESSED)
	{
		isActive = ((isMouseOver(event)) ? true : false);
	}
}

void EditText::draw(SDL_Renderer *ren)
{
	//Draw button
	SDL_RenderCopy(ren, frame, srcRect, dstRect);

	//Highlight button if nessesary
	if (isActive)
	{
		SDL_RenderCopy(ren, frameHighlight, srcRect, dstRect);
	}

	//Draw text
	if (content == "" && !isActive)
	{
		tooltip->draw(ren);
	}
	else
	{
		text->draw(ren);
	}

	objective->draw(ren);
}

bool EditText::isWriting()
{
	return isActive;
}

void EditText::setActive(bool state)
{
	isActive = state;
}

string EditText::getContent()
{
	return content;
}

void EditText::clearContent()
{
	content = "";
	text->setText(content);
}