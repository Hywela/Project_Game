#include "Combat.h"


Combat::Combat(){
	
}


//send ships from database with current stats and statuses to be able to continue from a previously started game
Combat::Combat(Space_Ship yourShip, Space_Ship enemyShip, bool yourTurn){
	you = yourShip;
	enemy = enemyShip;
	this->yourTurn = yourTurn;
	int yourHealth = 100;		//calculate health from ship modules and shields
	int enemyHealth = 100;	

	while(yourHealth > 0 && enemyHealth > 0){ //while both still allive
		if(yourTurn){
			makeMoves();
		}else{
			listenForMoves();
		}
	}
}

void Combat::makeMoves(){

}

void Combat::listenForMoves(){

}