#include "Network.h"


Network::Network()
{
		switch_char = 'm'; // default m has no function 
		id = 0;
		server_name = "128.39.169.189";  // This is the default server name, and creates enough memory to store other names from argv[1]
		port_number = 8880;  // This is a default port that is overridden by argv[2]
		default_client_port = 8881; // set to 0 for a random port

	
                fprintf(stderr, "Usage: udpclient host port (default: updclient %s %d ) \n", server_name, port_number);
        


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
	SDLNet_FreePacket(packet);
	SDLNet_Quit();
}

void Network :: handler_recive(){

	if (SDLNet_UDP_Recv(udpsock, packet))
		{
			switch_char='m';
			 data_from_packet= (char *)packet->data;
			 if (data_from_packet[0] == '*')
			  switch_char = data_from_packet[1];
			


			printf("UDP Packet incoming\n");
			printf("\tChan:    %d\n", packet->channel);
			printf("\tData:    %s\n", (char *)packet->data);
		//printf("\tINFO:    %s\n", (char)p->data[2]);
			printf("\tLen:     %d\n", packet->len);
			printf("\tMaxlen:  %d\n", packet->maxlen);
			printf("\tStatus:  %d\n", packet->status);
			printf("\tAddress: %x %x\n", packet->address.host, packet->address.port);

 switch (switch_char)
			{
			case 'a':{
					 
				
					 }
			break;
			case 'b':{
					 
					 
					 
					 
					 }
			break;
			case 'c':{
					 
					 
					
					 }
			break;
			case 'd':{
					 
					 //db->handler_set_online(1);
			
					 
					 
					 }
			break;
			case 'e':{ "";}
			break;
			case 'u':{ 

				std::cout << " package reviced";
			}
				
			break;
			default: SDL_Delay(20);
				break;
			}

 }
			 



	}
void Network::handler_send(char *input)
{
	

	strcpy_s((char *)packet->data, strlen(input)+1, input);
	
           
 
		packet->address.host = server_address.host;	/* Set the destination host */
		packet->address.port = server_address.port;	/* And destination port */
		
		packet->len = 24;
		SDLNet_UDP_Send(udpsock, -1, packet); /* This sets the p->channel */
 
		
	
	
 

}
