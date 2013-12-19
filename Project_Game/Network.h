#pragma once

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <vector>

using namespace std;
const unsigned short BUFFER_SIZE = 1024;


class Network
{
private:
	unsigned short PORT;		// The port we are connecting to
	const char *host;         // Where we store the host name
	IPaddress serverIP;       // The IP we will connect to
    TCPsocket clientSocket;   // The socket to use
	string serverName;     // The server name
	string userInput;    // A string to hold our user input
    int inputLength;     // The length of our string in characters
    char buffer[BUFFER_SIZE]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.
    bool shutdownClient,isServerOnline;
	int hostResolved;
	SDLNet_SocketSet socketSet;
	
public:
	Network(unsigned short port,string serverName);
	~Network();
	bool whoStarts();
	bool ifServerFoundIt(string testcase, int waitTime);	
	bool loggout();
	string reciveString(int waitTime);
	void checkServer();
	void send(string input);
	void recive();
	bool checkLogin();
    bool isServerOnlineMethod();
	void saveShip(string ship);
	void debug();
	string getShip();
	string getEnemyShip();
};

