#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <SDL.h>
#include <SDL_net.h>

class Network
{
		UDPsocket udpsock; /* Socket descriptor */
        IPaddress server_address; /* the server address */
        UDPpacket *packet; /* Pointer to packet memory */
        int quit;
        int x;
        int y;
		char * server_name;
		Uint16 port_number;
		Uint16 default_client_port;

public:
	Network(int argc, char** argv);
	~Network();
	void handler_init(char *server_name, Uint16 port);
};

