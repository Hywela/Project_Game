#include "Space_Ship.h"



Space_Ship::Space_Ship()
{
	//Not in use
}
Space_Ship::Space_Ship(SDL_Renderer *ren)
{
	//Set size and position
	const int TILE_OFFSET = 2;
	int renW, renH;
	SDL_GetRendererOutputSize(ren, &renW, &renH);
	position = new SDL_Rect();
	position->w = (SHIP_WIDTH * TILE_SIZE) + (SHIP_WIDTH * TILE_OFFSET);
	position->h = (SHIP_HEIGHT * TILE_SIZE) + (SHIP_HEIGHT * TILE_OFFSET);
	position->x = SDL_floor(renW / 2) - (position->w / 2);
	position->y = SDL_floor(renH / 2) - (position->h / 2);

	selected_hull = 0;


	//Calculate central tile
	int approxCenterX = (int)SDL_floor(SHIP_WIDTH / 2);
	int approxCenterY = (int)SDL_floor(SHIP_HEIGHT / 2);

	//Create rectangles
	SDL_Rect srcRect;
	srcRect.w = TILE_SIZE;
	srcRect.h = TILE_SIZE;
	srcRect.x = 0;
	srcRect.y = 0;
	SDL_Rect dstRect = srcRect;

	//Create all placeable tiles
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		dstRect.y = position->y + (srcRect.h * y) + (y * TILE_OFFSET);
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			dstRect.x = position->x + (srcRect.w * x) + (x * TILE_OFFSET);
			if (y == approxCenterY && x == approxCenterX)
			{
				//Set central tile (first tile)
				hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Normal.png", 1, 0, 0, true, false, false);
				int test = 0;
			}
			else
			{
				//Set empty tile
				hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Empty.png", 0, 0, 0, true, true, false);
			}
		}
	}

	//Set adjacent tiles placeable
	hull_layer[approxCenterY][approxCenterX - 1]->setPlaceable(true);
	hull_layer[approxCenterY][approxCenterX + 1]->setPlaceable(true);
	hull_layer[approxCenterY - 1][approxCenterX]->setPlaceable(true);
	hull_layer[approxCenterY + 1][approxCenterX]->setPlaceable(true);

	//Load space ship
	bool build_hull = true;

	while (build_hull)
	{
		//Clear renderer
		SDL_RenderClear(ren);

		//Draw all hull tiles
		for (int y = 0; y < SHIP_HEIGHT; y++)
		{
			for (int x = 0; x < SHIP_WIDTH; x++)
			{
				hull_layer[y][x]->drawBuild(ren);
			}
		}

	//Render screen
	SDL_RenderPresent(ren);

	build_hull = buildHull(ren);
	}
}

Space_Ship::~Space_Ship()
{
}

void Space_Ship::draw(SDL_Renderer *ren)
{
	//Draw hulls
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			hull_layer[y][x]->draw(ren);
		}
	}
	
	//Draw modules
}

bool Space_Ship::buildHull(SDL_Renderer *ren)
{
	bool build_hull = true;

	SDL_Event event;
	

	int selected_hull = 0;



	//Configure hulls
	while (SDL_PollEvent(&event))
	{
	//Check if a key was PRESSED
	if (event.key.state == SDL_PRESSED)
	{
		//Figure out what the key does
		SDL_Keycode key = event.key.keysym.sym;
			
		cout << "[BUILD-KEY]: ";
		switch (key)
		{
			case SDLK_RETURN:
			{
				cout << "Return";
				build_hull = false;
				break;
			}
				
			default:
			{
				cout << (char)key << " (Unhandeled key!)";
				break;
			}
		}
		cout << endl;
	}
	else if (event.button.state == SDL_PRESSED)
		{
			//Figure out what the key does
			SDL_Keycode button = event.button.button;
			
			cout << "[BUILD-EVENT]: ";
			switch (button)
			{
				case SDL_BUTTON_LEFT:
				{
					cout << "Left button\n";
					//Draw all hull tiles
					for (int y = 0; y < SHIP_HEIGHT; y++)
					{
						for (int x = 0; x < SHIP_WIDTH; x++)
						{
							//Grab the action when clicking on the button
							switch (hull_layer[y][x]->onMouseClick(event))
							{
								case PLACE:
								{
									swapHull(ren, x, y, selected_hull);
									break;
								}
								case MOVE:
								{
									break;
								}
								case REMOVE:
								{
									swapHull(ren, x, y, -1);
									break;
								}
								default:
								{
									break;
								}
							}
						}
					}
					break;
				}
				case SDL_BUTTON_RIGHT:
				{
					cout << "Right button\n";

					cout << "HullType before: " << selected_hull << endl;
					selected_hull = (++selected_hull) % 3;
					cout << "HullType = " << selected_hull << " (1 = NORMAL | 2 = ELECTRICAL | 3 = REINFORCED)\n";
					break;

					selected_hull = (selected_hull + 1) % 3;

				}
				default:
				{
					cout << button << " (Unhandeled button!)\n";
					break;
				}
			}
		}
	}

	return build_hull;
}

void Space_Ship::swapHull(SDL_Renderer *ren, int x, int y, int type)
{
	//Copy position
	SDL_Rect tmpSrc = hull_layer[y][x]->getSource();
	SDL_Rect tmpDst = hull_layer[y][x]->getDestination();
	int tmpNum = hull_layer[y][x]->getAnchors();

	//Remove old
	delete hull_layer[y][x];

	//Place new hull
	switch (hull_type(type))
	{
		case NORMAL:
		{
			hull_layer[y][x] = new Hull(ren, tmpSrc, tmpDst, DIR_HULLS + "Normal.png", 1, 0, 0, false, false, false);
			break;
		}
		case ELECTRICAL:
		{
			hull_layer[y][x] = new Hull(ren, tmpSrc, tmpDst, DIR_HULLS + "Electrical.png", 1, 1, 0, false, false, false);
			break;
		}
		case REINFORCED:
		{
			hull_layer[y][x] = new Hull(ren, tmpSrc, tmpDst, DIR_HULLS + "Reinforced.png", 1, 0, 1, false, false, false);
			break;
		}
		default:
		{
			hull_layer[y][x] = new Hull(ren, tmpSrc, tmpDst, DIR_HULLS + "Empty.png", 0, 0, 0, true, true, true);
			break;
		}
	}
	hull_layer[y][x]->setAnchors(tmpNum);

	//Update adjacent tiles
	if (type == -1)
	{
		if (x >= 1)
		{
			hull_layer[y][x - 1]->setLocked(false);
			hull_layer[y][x - 1]->setPlaceable(false);
		}
		if (x < SHIP_WIDTH - 1)
		{
			hull_layer[y][x + 1]->setLocked(false);
			hull_layer[y][x + 1]->setPlaceable(false);
		}
		if (y >= 1)
		{
			hull_layer[y - 1][x]->setLocked(false);
			hull_layer[y - 1][x]->setPlaceable(false);
		}
		if (y < SHIP_HEIGHT - 1)
		{
			hull_layer[y + 1][x]->setLocked(false);
			hull_layer[y + 1][x]->setPlaceable(false);
		}
	}
	else
	{
		if (x >= 1)
		{
			hull_layer[y][x - 1]->setLocked(true);
			hull_layer[y][x - 1]->setPlaceable(true);
		}
		if (x < SHIP_WIDTH - 1)
		{
			hull_layer[y][x + 1]->setLocked(true);
			hull_layer[y][x + 1]->setPlaceable(true);
		}
		if (y >= 1)
		{
			hull_layer[y - 1][x]->setLocked(true);
			hull_layer[y - 1][x]->setPlaceable(true);
		}
		if (y < SHIP_HEIGHT - 1)
		{
			hull_layer[y + 1][x]->setLocked(true);
			hull_layer[y + 1][x]->setPlaceable(true);
		}
	}
}

bool Space_Ship::buildModules(SDL_Renderer *ren)
{
	bool build_module = true;
	return build_module;
}