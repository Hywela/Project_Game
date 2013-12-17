#pragma once

#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>
#include <vector>

using namespace std;
const unsigned short PORT        = 6881; // The port we are connecting to
const unsigned short BUFFER_SIZE = 512;


class Network
{
private:
	const char *host;         // Where we store the host name
	IPaddress serverIP;       // The IP we will connect to
    TCPsocket clientSocket;   // The socket to use
	string serverName;     // The server name
	string userInput;    // A string to hold our user input
    int inputLength;     // The length of our string in characters
    char buffer[512]; // Array of character's we'll use to transmit our message. We get input into the userInput string for ease of use, then just copy it to this character array and send it.
    bool shutdownClient,isServerOnline;
	int hostResolved;
	SDLNet_SocketSet socketSet;
	
	

public:
	Network();
	~Network();
		
	bool loggout();
	void handler_check_server();
	void handler_send(string input);
	void handler_recive();
	bool handler_check_login();
    bool handler__is_server_online();
    bool handler_matchFound();

	void debug();
};

