#include "Combat.h"
/* Notes:
	- Send ships from database with current stats and statuses
	  to be able to continue from a previously started game.
*/


Combat::Combat()
{
}

Combat::Combat(Space_Ship yourShip, Space_Ship enemyShip, bool youStart)
{
	//Set game properties
	you = yourShip;
	enemy = enemyShip;
	yourTurn = youStart;

	//Ship health, calculates as a overall from modules. (Shields etc.)
	int yourHealth = 100;
	int enemyHealth = 100;

	//While both still alive
	while(yourHealth > 0 && enemyHealth > 0)
	{
		//Preform turn
		((yourTurn) ? makeMoves() : listenForMoves());
	}
}

void Combat::makeMoves(){
	//This players turn
	char dummy;
	cout << "\n------------------------------------\n"
		 << "Make a move: ";
	cin >> dummy;
	yourTurn = false;
}

void Combat::listenForMoves(){
	//The enemy's turn
	cout << "Waiting for enemy...\n";

	//Simulate waittime
	Sleep(2000);

	//Recieve answer
	cout << "Your enemy responded with: ...";
	yourTurn = true;
}