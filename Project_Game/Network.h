#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_net.h>

class Network
{
		UDPsocket udpsock; /* Socket descriptor */
        IPaddress server_address; /* the server address */
        UDPpacket *packet; /* Pointer to packet memory */
        int quit;
        int id;
		std::string username, password;
		std::string data_from_packet;
		char switch_char;
		char * server_name;
		Uint16 port_number;
		Uint16 default_client_port;

public:
	Network();
	~Network();
	void handler_init(char *server_name, Uint16 port);
	void handler_send(char *input);
	void handler_recive();
	bool handler_check_login();
};

