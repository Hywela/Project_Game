#include "Network.h"



Network::Network()
{   
    isServerOnline = true;
    serverName = "25.219.167.39";
	shutdownClient= false;
	 // Initialise SDL_net
    if (SDLNet_Init() < 0)
    {
        cout << "Failed to intialise SDN_net: " << SDLNet_GetError() << "\n";
        exit(-1); // Quit!
    }

	 socketSet = SDLNet_AllocSocketSet(1);
    if (socketSet == NULL)
    {

        cout << "Failed to allocate the socket set: " << SDLNet_GetError() << "\n";
        exit(-1); // Quit!
    }
    else
    {
        cout << "Successfully allocated socket set." << endl;
    }

    // Try to resolve the host. If successful, this places the connection details in the serverIP object
     hostResolved = SDLNet_ResolveHost(&serverIP, serverName.c_str(), PORT);
 
    if (hostResolved == -1)
    {
        cout << "Failed to resolve the server hostname: " << SDLNet_GetError() << "\nContinuing...\n";
    }
    else // If we successfully resolved the host then output the details
    {
        // Get our IP address in proper dot-quad format by breaking up the 32-bit unsigned host address and splitting it into an array of four 8-bit unsigned numbers...
        Uint8 * dotQuad = (Uint8*)&serverIP.host;
 
        //... and then outputting them cast to integers. Then read the last 16 bits of the serverIP object to get the port number
        cout << "Successfully resolved host to IP: " << (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1] << "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3];
        cout << " port " << SDLNet_Read16(&serverIP.port) << endl << endl;
    }
 
    // Try to resolve the IP of the server, just for kicks
    if ((host = SDLNet_ResolveIP(&serverIP)) == NULL)
    {
        cout << "Failed to resolve the server IP address: " << SDLNet_GetError() << endl;
       
    }
    else
    {
        cout << "Successfully resolved IP to host: " << host << endl;
    }
	
}

Network::~Network()
{
 SDLNet_TCP_Close(clientSocket);
 
    SDLNet_Quit();
}

void Network::handler_check_server()
{
	// Try to open a connection to the server and quit out if we can't connect
    clientSocket = SDLNet_TCP_Open(&serverIP);
    if (!clientSocket)          
    { cout << "Failed to open socket to server: " << SDLNet_GetError() << "\n";     //TODO: Adds some sort of waiting graphic
       isServerOnline=false;
    }
    else // If we successfully opened a connection then check for the server response to our connection
    {
        cout << "Connection okay, about to read connection status from the server..." << endl;
 
        // Add our socket to the socket set for polling
        SDLNet_TCP_AddSocket(socketSet, clientSocket);
 
        // Wait for up to five seconds for a response from the server
        // Note: If we don't check the socket set and WAIT for the response, we'll be checking before the server can respond, and it'll look as if the server sent us nothing back
        int activeSockets = SDLNet_CheckSockets(socketSet, 5000);
 
        cout << "There are " << activeSockets << " socket(s) with data on them at the moment." << endl;
		
        // Check if we got a response from the server
        int gotServerResponse = SDLNet_SocketReady(clientSocket);
	

        if (gotServerResponse != 0)
        {
            cout << "Got a response from the server... " << endl;
            int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
 
            cout << "Got the following from server: " << buffer << "(" << serverResponseByteCount << " bytes)" << endl;
 
            // We got an okay from the server, so we can join!
            if ( strcmp(buffer, "OK") == 0 )
            {
                // So set the flag to say we're not quitting out just yet
                shutdownClient = false;
 
                cout << "Joining server now..." << endl << endl;
            }
            else
            {
                cout << "Server is full... Terminating connection." << endl;
            }
        }
        else
        {
            cout << "No response from server..." << endl;
           
        }
 
    } // End of if we managed to open a connection to the server condition
}

void Network::handler_recive()
{


	int socketActive = SDLNet_CheckSockets(socketSet, 0);
 
        //cout << "Sockets with data on them at the moment: " << activeSockets << endl;
 
        if (socketActive != 0)
        {
            // Check if we got a response from the server
            int messageFromServer = SDLNet_SocketReady(clientSocket);
 
            if (messageFromServer != 0)
            {
                //cout << "Got a response from the server... " << endl;
                int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
 
                cout << "Received: " << buffer << endl;// "(" << serverResponseByteCount << " bytes)" << endl;
			
				if(strcmp(buffer, "function/")){
					string function_name;
					int value_one; // Parameter 1 
					int value_two;	// Parameter 2
					bool target;

				//	func_map_t::const_iterator function_map = func_map.find(function_name);
					//if( function_map == func_map.end() ) {}
				//	(*function_map->second)(value_one,value_two, target);
				}
				if (strcmp(buffer, "test/")){
					string in(buffer);
					vector<void (*)()> temp_vec;
					int idx = in.find("/");
					string tem = in.substr(idx+1);
					int temp = atoi(tem.c_str()); 
					_memccpy(&temp_vec, (const void *)temp, sizeof(temp), sizeof(temp));
					temp_vec[0]();
				}
                if (strcmp(buffer, "shutdown") == 0)
                {
                    cout << "Server is going down. Disconnecting..." << endl;
                    shutdownClient = true;
                }
            }
            else
            {
                //cout << "No response from server..." << endl;
            }
		}
}

bool Network::handler_check_login()
{


	if (handler_loggout()){
		return true;
	}

	return false;
}

void Network::handler_send(string input)
{   
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

// TODO :: Rename to check logout , write a quit function that tells the serer to disband connection to client
bool Network:: handler_loggout()
{

	bool check = false;
	 int socketActive = SDLNet_CheckSockets(socketSet, 6000);
 
        //cout << "Sockets with data on them at the moment: " << activeSockets << endl;
 
        if (socketActive != 0)
        {
            // Check if we got a response from the server
            int messageFromServer = SDLNet_SocketReady(clientSocket);
 
            if (messageFromServer != 0)
            {
                //cout << "Got a response from the server... " << endl;
                int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);
 
                cout << "Received: " << buffer << endl;
			
                if (strcmp(buffer, "OK")==0)
				{
					check = true;
				}else check = false;
            }
            else
            {check = false;
              
            }
		
        }
 return check;
}

bool Network:: is_server_online()
{


    return isServerOnline;
}

void Network:: debug(){
	
	string d;
	getline(cin, d);
	handler_send(d);

}