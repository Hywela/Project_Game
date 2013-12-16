#include "space_race.h"


space_race::space_race()
{
}


space_race::space_race(Space_Ship *yourShip, bool youStart, SDL_Renderer *rend, SDL_Window *wind)
{
	//Copy renderer and window
	ren = rend;
	win = wind;
	
	//Set up button properties
	int winH, winW;
	SDL_GetWindowSize(win, &winW, &winH);

	int btnWidth = 140;
	int btnHeight = 30;
	int startY = 220;
	int offsetY = 20;
	int btnX = (winW / 2) - (btnWidth / 2);
	int btnY = (winH / 8) - (btnHeight / 2);
	float scaleX = btnX;
	float scaleY = btnY;
	
	
	//Create texture from image
	string bgStr = DIR_BACKGROUNDS + "Space.png";
	background = IMG_LoadTexture(ren, bgStr.c_str());

	//Set game properties
	you = yourShip;

	yourTurn = youStart;

	//Position ships
	x = (winW / 4) - (((64 + 2) * 5) / 2);

	y = (winH / 2) ;

	you->setPosition(x, y);
	
	shooting = false;
	


	



	//Ship health, calculates as a overall from modules. (Shields etc.)
	 youAlive = true;
	while(youAlive)
	{
		//Preform turn
		listenForMoves();
	
	}

	//Game done, heal restore ships
	you->restore();

}

void space_race::makeMoves()
{
	
	 while( SDL_PollEvent( &event ) ){
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type ){

	case SDL_KEYDOWN:
		  switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        x -= 10;you->setPosition(x, y);
                        break;
                    case SDLK_RIGHT:
                        x += 10;you->setPosition(x, y);
                        break;
                    case SDLK_UP:
						bullets.push_front(new Bullet(ren,x,y-10));
                        break;
                    case SDLK_DOWN:
                   //     y += 5; 
						youAlive = false;
                        break;
                    default:
                        break;
                }
            }
        }
}

void space_race::listenForMoves(){

		makeMoves();
		draw();
		

}
void space_race::shoot(){
	Bullet *tem = NULL;
	for (int i=0; i<bullets.size(); i++){
		tem = bullets.back();
		tem->draw();//tem->shoot();
		  bullets.pop_back();
		if(!tem->reachedDest()){
			bullets.push_front(tem);
			
		}
	}
	//delete tem;
	
}
void space_race::draw()
{
	//Reset screen
	SDL_RenderClear(ren);

	//Draw background
	SDL_RenderCopy(ren, background, NULL, NULL);

	//Draw ships

	you->draw();
	shoot();
	

	

	//Render screen
	SDL_RenderPresent(ren);
}