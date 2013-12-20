#include "Network.h"
#include <windows.h>
#include <sstream>



Network::Network(unsigned short PORT ,string serverName)
{   
	whoStarts = false;
	this->PORT = PORT;
	isServerOnline = true;
	this->serverName = serverName;
	shutdownClient= false;

	// Initialise SDL_net
	if (SDLNet_Init() < 0){
		cout << "Failed to intialise SDN_net: " << SDLNet_GetError() << "\n";
		exit(-1); // Quit!
	}
	socketSet = SDLNet_AllocSocketSet(1);
	if (socketSet == NULL){
		cout << "Failed to allocate the socket set: " << SDLNet_GetError() << "\n";
		exit(-1); // Quit!
	}
	else{
		cout << "Successfully allocated socket set." << endl;
	}

	// Try to resolve the host. If successful, this places the connection details in the serverIP object
	hostResolved = SDLNet_ResolveHost(&serverIP, serverName.c_str(), PORT);
	if (hostResolved == -1){
		cout << "Failed to resolve the server hostname: " << SDLNet_GetError() << "\nContinuing...\n";
	}
	else{ // If we successfully resolved the host then output the details
		// Get our IP address in proper dot-quad format by breaking up the 32-bit unsigned host address and splitting it into an array of four 8-bit unsigned numbers...
		Uint8 * dotQuad = (Uint8*)&serverIP.host;
		//... and then outputting them cast to integers. Then read the last 16 bits of the serverIP object to get the port number
		cout << "Successfully resolved host to IP: " << (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1] << "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3];
		cout << " port " << SDLNet_Read16(&serverIP.port) << endl << endl;
	}

	// Try to resolve the IP of the server, just for kicks
	if ((host = SDLNet_ResolveIP(&serverIP)) == NULL){
		cout << "Failed to resolve the server IP address: " << SDLNet_GetError() << endl;
	}
	else{
		cout << "Successfully resolved IP to host: " << host << endl;
	}
}

Network::~Network()
{
	SDLNet_TCP_Close(clientSocket);
	SDLNet_Quit();
}

//Methods
void Network::checkServer()
{
	// Try to open a connection to the server and quit out if we can't connect
	clientSocket = SDLNet_TCP_Open(&serverIP);
	if (!clientSocket){ 
	cout << "Failed to open socket to server: " << SDLNet_GetError() << "\n";     //TODO: Adds some sort of waiting graphic
	isServerOnline=false;
	}
	else{ // If we successfully opened a connection then check for the server response to our connection
		cout << "Connection okay, about to read connection status from the server..." << endl;

		// Add our socket to the socket set for polling
		SDLNet_TCP_AddSocket(socketSet, clientSocket);

		// Wait for up to five seconds for a response from the server
		// Note: If we don't check the socket set and WAIT for the response, we'll be checking before the server can respond, and it'll look as if the server sent us nothing back
		int activeSockets = SDLNet_CheckSockets(socketSet, 5000);

		cout << "There are " << activeSockets << " socket(s) with data on them at the moment." << endl;

		// Check if we got a response from the server
		int gotServerResponse = SDLNet_SocketReady(clientSocket);
		if (gotServerResponse != 0){
			cout << "Got a response from the server... " << endl;
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);

			cout << "Got the following from server: " << buffer << "(" << serverResponseByteCount << " bytes)" << endl;

			// We got an okay from the server, so we can join!
			if ( strcmp(buffer, "OK") == 0 ){
				// So set the flag to say we're not quitting out just yet
				shutdownClient = false;
				cout << "Joining server now..." << endl << endl;
			}
			else{
				cout << "Server is full... Terminating connection." << endl;
			}
		}
		else{
			cout << "No response from server..." << endl;
		}
	} // End of if we managed to open a connection to the server condition
}

//Recive Methods
void Network::recive(){
	// recives data from the server
	int socketActive = SDLNet_CheckSockets(socketSet, 0);
	if (socketActive != 0)
	{
		// Check if we got a response from the server
		int messageFromServer = SDLNet_SocketReady(clientSocket);
		if (messageFromServer != 0){
			//cout << "Got a response from the server... " << endl;
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
			//cout << "Received: " << buffer << endl;
		}
		if (strcmp(buffer, "shutdown") == 0){
			cout << "Server is going down. Disconnecting..." << endl;
			shutdownClient = true;
		}
	}
	else{
		//cout << "No response from server..." << endl;
	}
}//End recive();

string Network::reciveString(int waitTime){
	// Gets the whole buffer from the server and returns a string witht he buffer
	int socketActive = SDLNet_CheckSockets(socketSet, waitTime);
	if (socketActive != 0){
		// Check if we got a response from the server
		int messageFromServer = SDLNet_SocketReady(clientSocket);
		if (messageFromServer != 0){ //if there is a message
			//cout << "Got a response from the server... " << endl;
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
			string temp(buffer);
			return temp;
		}else {//End-meesageFromServer
			return "-1";
		}
	}else {//End-SocketActivity(
		return "-1";}
}//End Method reciveString(..);

bool Network::ifServerFoundIt(string testCase, int waitTime){
	// takes a test settence and checks if the server has recived that pakage
	// Wait time is the time the socket should wait on the package
	int socketActive = SDLNet_CheckSockets(socketSet, waitTime);
	if (socketActive != 0) {
		// Check if we got a response from the server
		int messageFromServer = SDLNet_SocketReady(clientSocket);
		if (messageFromServer != 0) { // any respons ?
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
			if (strcmp(buffer, testCase.c_str())== 0){ //is the testCase = 0 (true)
				//cout << "\n "<< testCase << "found";  
				return true;
			}//end - compare
		}//end - message from server
	}// end socketActivity
	return false;
}//end - ifServerFoundIt(..);

bool Network::checkLogin()
{	//Return true or false;
	return ifServerFoundIt("OK",2000);
}

//Send Methods
void Network::send(string input){   

	// Copy our user's string into our char array called "buffer"
	strcpy_s( buffer,input.c_str() );
	inputLength = strlen(buffer) + 1;
	// Calculate the length of our input and then add 1 (for the terminating character) to get the total number of characters we need to send
	// Send the message to the server
	if (SDLNet_TCP_Send(clientSocket, (void *)buffer, inputLength) < inputLength)
	{
		cout << "Failed to send message: " << SDLNet_GetError() << endl;
		exit(-1);
	}	
}

// random method
bool Network::isServerOnlineMethod(){
	return isServerOnline; //well is it online ?
}

//ship saving and getting methods
void Network::saveShip(string ship){
	string shipToGO = "b "; // send the command b to ask it to save the string 
	shipToGO.append(ship); // appends ship Data into the string
	send(shipToGO);		// sends it into deep space
}

string Network::getShip(){
	send("c");	//recives a a ship back after requesting it with command "c";
	return reciveString(1000);
}

string Network:: getEnemyShip(){
	send("e");// Gets the enemy ship from the server
	return reciveString(1000);
}

bool Network:: matchFound(){ // Who starts the match 
		string attackStr = reciveString(0);
		
		if (attackStr == "matchFound/YouAreStarting") { //TODO:: Fiks so it wont have to loop 
		
			 whoStarts = true; 
		return true;
		}if (attackStr == "matchFound/EnemyStarting") { //TODO:: Fiks so it wont have to loop 
			 whoStarts = false;
			 return true;
	}
		return false;
}

//Debug Methods
void Network::debug(){
	// method for manuly sending packages
	// NOT IN USE
	string input;
	getline(cin, input);
	send(input);
}

bool Network::starting(){

	return whoStarts;
}