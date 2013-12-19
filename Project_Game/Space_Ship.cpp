#include "Space_Ship.h"



Space_Ship::Space_Ship()
{
	//Not in use
}

Space_Ship::Space_Ship(SDL_Renderer *rend, SDL_Texture *bg, string title)
{
	ren = rend;
	buildBackground = bg;

	//Make instructions
	int basePosX = 0;
	int basePosY = 0;
	int baseOffset = 10;
	titleText = new Text(ren, title);
	titleText->setPosition(basePosX, basePosY);

	int tutorialPosY = basePosY + titleText->getHeight() + baseOffset;
	tutorialHull = new Text(ren, "Build hull:\n =LEFT MOUSE= Place or remove hull.\n =RIGHT MOUSE= Change material.\n =ENTER= to continue...");
	tutorialModule = new Text(ren, "Build module:\n =LEFT MOUSE= Place or remove module.\n =RIGHT MOUSE= Change module.\n =ENTER= to save...");
	tutorialHull->setPosition(basePosX, tutorialPosY);
	tutorialModule->setPosition(basePosX, tutorialPosY);

	int selectedPosY = tutorialPosY + tutorialHull->getHeight() + baseOffset;
	tutorialHullSelected = new Text(ren, "Selected hull:");
	tutorialModuleSelected = new Text(ren, "Selected module:");
	tutorialHullSelected->setPosition(basePosX, selectedPosY);
	tutorialModuleSelected->setPosition(basePosX, selectedPosY);

	//Load tutorial images
	tutorialTextureSrc = new SDL_Rect();
	tutorialTextureSrc->w = TILE_SIZE;
	tutorialTextureSrc->h = TILE_SIZE;
	tutorialTextureSrc->x = 0;
	tutorialTextureSrc->y = 0;

	tutorialTextureDst = new SDL_Rect();
	tutorialTextureDst->w = TILE_SIZE;
	tutorialTextureDst->h = TILE_SIZE;
	tutorialTextureDst->x = 0;
	tutorialTextureDst->y = selectedPosY + tutorialModuleSelected->getHeight();

	string tmpStr = DIR_HULLS + "Normal.png";
	hullTextures.push_back(IMG_LoadTexture(ren, tmpStr.c_str()));
	tmpStr = DIR_HULLS + "Electrical.png";
	hullTextures.push_back(IMG_LoadTexture(ren, tmpStr.c_str()));
	tmpStr = DIR_HULLS + "Reinforced.png";
	hullTextures.push_back(IMG_LoadTexture(ren, tmpStr.c_str()));

	tmpStr = DIR_MODULES + "Turret.png";
	moduleTextures.push_back(IMG_LoadTexture(ren, tmpStr.c_str()));
	tmpStr = DIR_MODULES + "Shield_Off.png";
	moduleTextures.push_back(IMG_LoadTexture(ren, tmpStr.c_str()));

	//Set size and position
	const int TILE_OFFSET = 2;
	int renW, renH;
	SDL_GetRendererOutputSize(ren, &renW, &renH);
	position = new SDL_Rect();
	position->w = (SHIP_WIDTH * TILE_SIZE) + (SHIP_WIDTH * TILE_OFFSET);
	position->h = (SHIP_HEIGHT * TILE_SIZE) + (SHIP_HEIGHT * TILE_OFFSET);
	position->x = renW - position->w - TILE_SIZE;
	position->y = SDL_floor(renH / 2) - (position->h / 2);
	selected_hull = NORMAL;
	selected_module = TURRET;

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
	build_hull = true;

	while (build_hull)
	{
		drawBuild();
		build_hull = buildHull();
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

	build_module = true;

	while (build_module)
	{
		drawBuild();
		build_module = buildModules();
	}

	//Determine energy
	energyMax = 10;
	energy = energyMax;

	//Initialize target
	target = NULL;
	computer = false;
	isDestroyed = false;
}

Space_Ship::Space_Ship(SDL_Renderer *rend, string structure)
{
	ren = rend;

	//Set size and position
	const int TILE_OFFSET = 2;
	int renW, renH;
	SDL_GetRendererOutputSize(ren, &renW, &renH);
	position = new SDL_Rect();
	position->w = (SHIP_WIDTH * TILE_SIZE) + (SHIP_WIDTH * TILE_OFFSET);
	position->h = (SHIP_HEIGHT * TILE_SIZE) + (SHIP_HEIGHT * TILE_OFFSET);
	position->x = renW - position->w - TILE_SIZE;
	position->y = SDL_floor(renH / 2) - (position->h / 2);

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

	//Gather planned parts
	vector <string> parts;
	string currentArg = "";

	//Decode attack
	for (char &c : structure)
	{
		if (c != ' ')
		{
			//Build string
			currentArg += c;
		}
		else
		{
			//Store string and reset
			parts.push_back(currentArg);
			currentArg = "";
		}
	}

	//Store last argument
	parts.push_back(currentArg);

	//Load ship hulls
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		dstRect.y = position->y + (srcRect.h * y) + (y * TILE_OFFSET);
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			dstRect.x = position->x + (srcRect.w * x) + (x * TILE_OFFSET);
			int hullId = 1 + (y * SHIP_WIDTH * 2) + (x * 2);
			int moduleId = hullId + 1;

			//Place new hull
			switch (hull_type(atoi(parts[hullId].c_str())))
			{
				case NORMAL:
				{
					hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Normal.png", 1, 0, 0, false, false, false);
					break;
				}
				case ELECTRICAL:
				{
					hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Electrical.png", 1, 1, 0, false, false, false);
					break;
				}
				case REINFORCED:
				{
					hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Reinforced.png", 1, 0, 1, false, false, false);
					break;
				}
				default:
				{
					hull_layer[y][x] = new Hull(ren, srcRect, dstRect, DIR_HULLS + "Empty.png", 0, 0, 0, true, true, true);
					break;
				}
			}

			//Place new module
			switch (module_type(atoi(parts[moduleId].c_str())))
			{
				case SHIELD:
				{
					module_layer[y][x] = new Module(ren, srcRect, dstRect, DIR_MODULES + "Shield_Off.png", 2, 10, 0, hull_layer[y][x]->getType(), 5, 0, 0, 3);
					break;
				}
				case TURRET:
				{
					module_layer[y][x] = new Module(ren, srcRect, dstRect, DIR_MODULES + "Turret.png", 1, 10, 0, hull_layer[y][x]->getType(), 4, 5);
					break;
				}
				default:
				{
					module_layer[y][x] = new Module(ren, srcRect, dstRect, DIR_MODULES + "Empty.png", 0, 0, 0);
					break;
				}
			}
		}
	}

	//Determine energy
	energyMax = 10;
	energy = energyMax;

	//Initialize target
	target = NULL;
	computer = false;
	isDestroyed = false;
}

Space_Ship::~Space_Ship()
{
}

void Space_Ship::draw()
{
	//Draw the ship parts
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The hulls
			if (hull_layer[y][x] != NULL)
			{
				hull_layer[y][x]->draw();
			}

			//The modules
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->draw(computer);
			}
		}
	}

	//Draw the interface
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The modules
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->drawInterface();
			}
		}
	}
}

bool Space_Ship::buildHull()
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
									swapHull(x, y, selected_hull);
									break;
								}
								case HUL_REMOVE:
								{
									swapHull(x, y, -1);
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
					selected_hull = hull_type((selected_hull + 1) % HULL_COUNT);
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

void Space_Ship::swapHull(int x, int y, int type)
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

bool Space_Ship::buildModules()
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
										swapModule(x, y, selected_module);
										break;
									}
									case HUL_REMOVE:
									{
										swapModule(x, y, -1);
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
					selected_module = module_type((selected_module + 1) % MODULE_COUNT);
					cout << "ModuleType = " << (selected_module + 1) << " (1: TURRET | 2: SHIELD)\n";
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

void Space_Ship::swapModule(int x, int y, int type)
{
	//Copy position
	SDL_Rect tmpSrc = hull_layer[y][x]->getSource();
	SDL_Rect tmpDst = hull_layer[y][x]->getDestination();

	//Remove old
	delete module_layer[y][x];

	//Place new module
	switch (module_type(type))
	{
		case SHIELD:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Shield_Off.png", 2, 10, 0, hull_layer[y][x]->getType(), 5, 0, 0, 3);
			break;
		}
		case TURRET:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Turret.png", 1, 10, 0, hull_layer[y][x]->getType(), 4, 5);
			break;
		}
		default:
		{
			module_layer[y][x] = new Module(ren, tmpSrc, tmpDst, DIR_MODULES + "Empty.png", 0, 0, 0);
			break;
		}
	}
}

void Space_Ship::setPosition(int posX, int posY)
{
	//Calculate change
	int changeX = posX - position->x;
	int changeY = posY - position->y;

	//Reposition the ship parts
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The hulls
			if (hull_layer[y][x] != NULL)
			{
				hull_layer[y][x]->setPosition(changeX, changeY);
			}

			//The modules
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->setPosition(changeX, changeY);
			}
		}
	}

	//Store new position
	position->x = posX;
	position->y = posY;
}

void Space_Ship::onMouseEvent(SDL_Event event)
{
	//Connect all the listeners
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The modules
			if (module_layer[y][x] != NULL)
			{
				bool over = module_layer[y][x]->isMouseOver(event);

				//Button down
				if (event.button.type == SDL_MOUSEBUTTONDOWN)
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (over)
						{
							module_layer[y][x]->setHeld(true);
						}
					}
				}
				//Button up
				else if (event.button.type == SDL_MOUSEBUTTONUP)
				{
					if (event.button.button == SDL_BUTTON_LEFT)
					{
						if (!module_layer[y][x]->hasTarget())
						{
							//If there is no target, clear the line
							module_layer[y][x]->clearTarget();
						}
						module_layer[y][x]->setHeld(false);

						if (energy && over)
						{
							if (module_layer[y][x]->addEnergy())
							{
								energy--;
							}
						}
					}
					else if (event.button.button == SDL_BUTTON_RIGHT && over)
					{
						if (module_layer[y][x]->removeEnergy())
						{
							energy++;
						}
					}
					//cout << "Energy: " << energy << " / 10\n";
				}
				else if (event.button.type == SDL_MOUSEMOTION && !over)
				{
					//Check if hovering a target
					int tarX, tarY, tarPosX, tarPosY;
					target->getHoveredModule(event, tarX, tarY, tarPosX, tarPosY);

					if (module_layer[y][x]->getHeld())
					{
						if (tarX != -1)
						{
							module_layer[y][x]->setTarget(tarX, tarY, tarPosX, tarPosY);
						}
						else
						{
							module_layer[y][x]->clearTarget();
						}

						//Draw target line
						module_layer[y][x]->setTargetLineToMouse(event.button.x, event.button.y);
					}
				}
			}
		}
	}
}

void Space_Ship::resetEnergy()
{
	//Clear old energy
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The modules
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->resetEnergy();
			}
		}
	}

	//Set new energy
	energy = energyMax;
}

void Space_Ship::attack(int posX, int posY, int dmg)
{
	//The modules
	if (module_layer[posY][posX] != NULL)
	{
		//Calculate actual damage
		int actualDamage = dmg - module_layer[posY][posX]->getDefence();

		//Check if module is shielded
		if  (	(module_layer[posY][posX - 1] != NULL && module_layer[posY][posX - 1]->isShielding())
			||	(module_layer[posY - 1][posX] != NULL && module_layer[posY - 1][posX]->isShielding())
			||	(module_layer[posY][posX + 1] != NULL && module_layer[posY][posX + 1]->isShielding())
			||	(module_layer[posY + 1][posX] != NULL && module_layer[posY + 1][posX]->isShielding())
			||	(module_layer[posY][posX] != NULL && module_layer[posY][posX]->isShielding()))
			{
				cout << "The attack was shielded!\n";
			}
		//Check if the module is partly shielded (50% damage taken)
		else if((module_layer[posY - 1][posX - 1] != NULL && module_layer[posY - 1][posX - 1]->isShielding())
			||	(module_layer[posY - 1][posX + 1] != NULL && module_layer[posY - 1][posX + 1]->isShielding())
			||	(module_layer[posY + 1][posX - 1] != NULL && module_layer[posY + 1][posX - 1]->isShielding())
			||	(module_layer[posY + 1][posX + 1] != NULL && module_layer[posY + 1][posX + 1]->isShielding()))
			{
				module_layer[posY][posX]->onHit(int(actualDamage / 2));
			}
		else
		{
			module_layer[posY][posX]->onHit(actualDamage);
		}
	}

	//Check if the ship is dead
	checkModuleHealth();
}

vector <string> Space_Ship::activate()
{
	vector <string> attacks;

	//Clear old energy
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The modules
			if (module_layer[y][x] != NULL)
			{
				//Check for energy updates
				int energyUpdates = module_layer[y][x]->getChangedEnergy();
				for (int i = 0; i < energyUpdates; i++)
				{
					string energyStr = "Power " + to_string(x) + " " + to_string(y);
					attacks.push_back(energyStr);
				}

				//Check for activation updates
				if (module_layer[y][x]->canActivate())
				{
					if (module_layer[y][x]->hasTarget())
					{
						string attack = module_layer[y][x]->registerAttack(x, y);
						if (attack != "")
						{
							attacks.push_back(attack);
						}
					}
				}
			}
		}
	}

	return attacks;
}

void Space_Ship::setTarget(Space_Ship *tar)
{
	target = tar;
}

void Space_Ship::getHoveredModule(SDL_Event event, int &x1, int &y1, int &posX, int &posY)
{
	x1 = -1;
	y1 = -1;
	posX = -1;
	posY = -1;

	//Clear old energy
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//The modules
			if (module_layer[y][x] != NULL)
			{
				if (module_layer[y][x]->isMouseOver(event) && module_layer[y][x]->getCurrentHealth())
				{
					x1 = x;
					y1 = y;
					module_layer[y][x]->getPosition(posX, posY);
				}
			}
		}
	}
}

void Space_Ship::setComputer(bool state)
{
	computer = state;
}

void Space_Ship::getEnergyLevel(int &cur, int &tot)
{
	cur = energy;
	tot = energyMax;
}

void Space_Ship::checkModuleHealth()
{
	//Check if all modules
	isDestroyed = true;
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			//Check if the module is alive
			if (module_layer[y][x] != NULL)
			{
				if (!module_layer[y][x]->isDead())
				{
					isDestroyed = false;
				}
			}
		}
	}
}

bool Space_Ship::isDead()
{
	return isDestroyed;
}

void Space_Ship::restore()
{
	//Restore all modules
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			if (module_layer[y][x] != NULL)
			{
				module_layer[y][x]->restore();
			}
		}
	}

	isDestroyed = false;
}

Module* Space_Ship::getModule(int row, int collumn){
	return module_layer[row][collumn];
}

Hull* Space_Ship::getHull(int row, int collumn){
	return hull_layer[row][collumn];
}

int Space_Ship::getMaxEnergy(){
	return energyMax;
}

void Space_Ship::drawBuild()
{
	//Clear renderer
	SDL_RenderClear(ren);

	//Draw background
	SDL_RenderCopy(ren, buildBackground, NULL, NULL);

	//Draw title
	titleText->draw();

	if (build_hull)
	{
		//Draw all hull tiles
		for (int y = 0; y < SHIP_HEIGHT; y++)
		{
			for (int x = 0; x < SHIP_WIDTH; x++)
			{
				hull_layer[y][x]->drawBuild();
			}
		}

		//Draw tutorial
		tutorialHull->draw();
		tutorialHullSelected->draw();

		//Draw selected tile
		SDL_RenderCopy(ren, hullTextures[selected_hull], tutorialTextureSrc, tutorialTextureDst);
	}
	else if (build_module)
	{
		//Draw all hull tiles
		for (int y = 0; y < SHIP_HEIGHT; y++)
		{
			for (int x = 0; x < SHIP_WIDTH; x++)
			{
				if (module_layer[y][x] != NULL)
				{
					hull_layer[y][x]->draw();
					module_layer[y][x]->draw(computer);
				}
			}
		}

		//Draw tutorial
		tutorialModule->draw();
		tutorialModuleSelected->draw();

		//Draw selected tile
		SDL_RenderCopy(ren, moduleTextures[selected_module], tutorialTextureSrc, tutorialTextureDst);
	}

	//Render screen
	SDL_RenderPresent(ren);
}

string Space_Ship::getShipStructure()
{
	string structure = "Ship";

	//Go through all hulls
	for (int y = 0; y < SHIP_HEIGHT; y++)
	{
		for (int x = 0; x < SHIP_WIDTH; x++)
		{
			if (hull_layer[y][x] != NULL && hull_layer[y][x]->getType() != -1)
			{
				//Add hull
				structure += " " + to_string(hull_layer[y][x]->getType());

				if (module_layer[y][x] != NULL && module_layer[y][x]->getType() != -1)
				{
					//Add module
					structure += " " + to_string(module_layer[y][x]->getType());
				}
				else
				{
					structure += " -1";
				}
			}
			else
			{
				//Has no hull or module
				structure += " -1 -1";
			}
		}
	}

	return structure;
}