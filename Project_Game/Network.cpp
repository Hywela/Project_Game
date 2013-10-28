#include "Network.h"


Network::Network(int argc, char** argv)
{
		x=23;
		y=15;
		server_name = "127.000.000.001";  // This is the default server name, and creates enough memory to store other names from argv[1]
		port_number = 8880;  // This is a default port that is overridden by argv[2]
		default_client_port = 8881; // set to 0 for a random port

	/* Check for parameters        if there are three overwrite otherwise we will be using default values */
        if (argc == 3)
        {
                server_name = argv[1];
                port_number = atoi(argv[2]);
        } else {
                fprintf(stderr, "Usage: udpclient host port (default: updclient %s %d ) \n", server_name, port_number);
        }


        /* Initialize SDL */
        if (SDL_Init(0) < 0)
        {
                fprintf(stderr, "SDL_Init: \n");
                exit(EXIT_FAILURE);
        }

        /* Initialize SDL_net */
        if (SDLNet_Init() < 0)
        {
                fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
        }
 
        /* Open a socket */
        if (!(udpsock = SDLNet_UDP_Open(default_client_port)))
        {
                fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
        }
 
                /* Open a socket on random port */
        if (!(udpsock = SDLNet_UDP_Open(0)))
        {
                fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
        }
 
        /* Resolve server name into server address */
        if (SDLNet_ResolveHost(&server_address, server_name, port_number) == -1)
        {
                fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n",server_name ,port_number, SDLNet_GetError());
                exit(EXIT_FAILURE);
        }


        /* Make space for the packet */
        if (!(packet = SDLNet_AllocPacket(512)))
        {
                fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
                exit(EXIT_FAILURE);
        }
}


Network::~Network()
{
}
