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
	selected_module = 0;

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

	//Reset rectangles
	dstRect = srcRect;

	//Find all possible module slots
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		dstRect.y = position->y + (srcRect.h * y) + (y * TILE_OFFSET);
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			dstRect.x = position->x + (srcRect.w * x) + (x * TILE_OFFSET);
			if (hull_layer[y][x]->isNotUsed())
			{
				//Delete the unused hull
				delete hull_layer[y][x];
				hull_layer[y][x] = NULL;

				//This tile can't be used for modules either
				module_layer[y][x] = NULL;
			}
			else
			{
				//Set empty module
				module_layer[y][x] = new Module(ren, srcRect, dstRect, DIR_MODULES + "Empty.png", 1, 0, 0);
			}
		}
	}

	bool build_module = true;

	while (build_module)
	{
		//Clear renderer
		SDL_RenderClear(ren);

		//Draw all hull tiles
		for (int y = 0; y < SHIP_HEIGHT; y++)
		{
			for (int x = 0; x < SHIP_WIDTH; x++)
			{
				if (module_layer[y][x] != NULL)
				{
					hull_layer[y][x]->draw(ren);
					module_layer[y][x]->draw(ren);
				}
			}
		}

		//Render screen
		SDL_RenderPresent(ren);

		build_module = buildModules(ren);
	}
}

Space_Ship::~Space_Ship()
{
}

void Space_Ship::draw(SDL_Renderer *ren)
{
	//Draw the ship parts
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The hulls
			if (hull_layer[y][x] != NULL)
			{
				hull_layer[y][x]->draw(ren);
			}

			//The modules
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->draw(ren);
			}
		}
	}
}

bool Space_Ship::buildHull(SDL_Renderer *ren)
{
	bool build_hull = true;
	SDL_Event event;
	

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
								case HUL_PLACE:
								{
									swapHull(ren, x, y, selected_hull);
									break;
								}
								case HUL_REMOVE:
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
					selected_hull = (++selected_hull) % 3;
					cout << "HullType = " << (selected_hull + 1) << " (1 = NORMAL | 2 = ELECTRICAL | 3 = REINFORCED)\n";
					break;
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
	SDL_Event event;
	

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
				build_module = false;
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
							if (module_layer[y][x] != NULL)
							{
								switch (module_layer[y][x]->onMouseClick(event))
								{
									case HUL_PLACE:
									{
										swapModule(ren, x, y, selected_module);
										break;
									}
									case HUL_REMOVE:
									{
										swapModule(ren, x, y, -1);
										break;
									}
									default:
									{
										break;
									}
								}
							}
						}
					}
					break;
				}
				case SDL_BUTTON_RIGHT:
				{
					cout << "Right button\n";
					selected_module = (++selected_module) % 2;
					cout << "ModuleType = " << (selected_module + 1) << " (1 = COCKPIT | 2 = TURRET)\n";
					break;
				}
				default:
				{
					cout << button << " (Unhandeled button!)\n";
					break;
				}
			}
		}
	}

	return build_module;
}

void Space_Ship::swapModule(SDL_Renderer *ren, int x, int y, int type)
{
	//Copy position
	SDL_Rect tmpSrc = hull_layer[y][x]->getSource();
	SDL_Rect tmpDst = hull_layer[y][x]->getDestination();

	//Remove old
	delete module_layer[y][x];

	//Place new hull
	switch (module_type(type))
	{
		case COCKPIT:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Cockpit.png", 1, 10, 0);
			break;
		}
		case TURRET:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Turret.png", 1, 10, 0);
			break;
		}
		default:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Empty.png", 1, 0, 0);
			break;
		}
	}
}