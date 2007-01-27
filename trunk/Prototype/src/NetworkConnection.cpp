#include "NetworkConnection.h"

namespace Prototype
{	
	void NetworkConnection::openClientConnection1()
	{
		openClientConnection(clientSocket1);
	}

	void NetworkConnection::openClientConnection2()
	{
		openClientConnection(clientSocket2);
	}

	void NetworkConnection::openClientConnection(TCPsocket clientSocket)
	{
		IPaddress ip;
		char message[1024];
		int len;
		Uint16 port = 12333;

		// Resolve the argument into an IPaddress type
		if (SDLNet_ResolveHost(&ip, "localhost", port) == -1)
		{
			printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
			exit(3);
		}

		// open the server socket
		clientSocket = SDLNet_TCP_Open(&ip);
		if(!clientSocket)
		{
			printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
			exit(4);
		}
		/*
		// read the buffer from stdin
		printf("Enter Message, or Q to make the server quit:\n");
		fgets(message, 1024, stdin);
		len = strlen(message);

		// strip the newline
		message[len-1]='\0';
		
		if(len)
		{
			int result;
			
			// print out the message
			printf("Sending: %.*s\n",len,message);

			result=SDLNet_TCP_Send(sock,message,len); // add 1 for the NULL
			if(result<len)
				printf("SDLNet_TCP_Send: %s\n",SDLNet_GetError());
		}
		*/

	}
	
	void NetworkConnection::closeClientConnection()
	{
		SDLNet_TCP_Close(clientSocket1);
		SDLNet_TCP_Close(clientSocket2);
	}
	
	
	void NetworkConnection::startServer()
	{
		IPaddress ip;
		numberOfClients = 0;

		// Create a server type IPaddress
		if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
		{
			printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			exit(3);
		}

		// open the server socket
		serverSocket = SDLNet_TCP_Open(&ip);
		if(!serverSocket)
		{
			printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			exit(4);
		}
	}
	
	void NetworkConnection::createSocketSet()
	{
		if (set)
		{
			SDLNet_FreeSocketSet(set);
		}

		set = SDLNet_AllocSocketSet(numberOfClients + 1);
		
		if (!set)
		{
			printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
			exit(1);
		}
		
		SDLNet_TCP_AddSocket(set, serverSocket);
		
		for (size_t i = 0; i < numberOfClients; i++)
		{
			SDLNet_TCP_AddSocket(set, clients[i]);
		}
	}
	
	// returns true if client got connected
	bool NetworkConnection::waitForClient()
	{
		IPaddress *remoteip;
		createSocketSet();
		size_t numReady = SDLNet_CheckSockets(set, -1);

		if (numReady == -1)
		{
			// error
			return false;
		}
		
		if (numReady == 0)
		{
			return false;
		}
		
		
		//char message[1024];
		//int len;
		//Uint32 ipaddr;

		// check for new connection from a client
		if (SDLNet_SocketReady(serverSocket))
//		if (true)
		{
			// try to accept a connection
			serverClientSocket = SDLNet_TCP_Accept(serverSocket);

			if(!serverClientSocket)
			{ // no connection accepted
				printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
				SDL_Delay(100); //sleep 1/10th of a second
				return false;
			}
			
			// get the clients IP and port number
			remoteip = SDLNet_TCP_GetPeerAddress(serverClientSocket);

			if(!remoteip)
			{
				printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
				return false;
			}

			// print out the clients IP and port number
			Uint32 ipaddr = SDL_SwapBE32(remoteip->host);
			printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
				ipaddr>>24,
				(ipaddr>>16)&0xff,
				(ipaddr>>8)&0xff,
				ipaddr&0xff,
				remoteip->port);
				
			clients.push_back(serverClientSocket);
			numberOfClients++;
			
			return true;
		}
		
		/*
		// read the buffer from client
		len = SDLNet_TCP_Recv(client,message,1024);
		SDLNet_TCP_Close(client);
		if(!len)
		{
			printf("SDLNet_TCP_Recv: %s\n",SDLNet_GetError());
			continue;
		}

		// print out the message
		printf("Received: %.*s\n",len,message);

		if(message[0]=='Q')
		{
			printf("Quitting on a Q received\n");
			break;
		}
		*/
	}

	void NetworkConnection::closeServer()
	{
		Clients::iterator it = clients.begin();
		Clients::iterator end = clients.end();
		
		for ( ; it != end; ++it)
		{
			TCPsocket socket = *it;
		
			SDLNet_TCP_Close(socket);
		}
		
		SDLNet_FreeSocketSet(set);
	
	}
};