#include "NetworkConnection.h"

namespace Prototype
{	
	//-----------------------------------------------------------
	// NETWORK MESSAGE SENDER
	//-----------------------------------------------------------
	void NetworkMessageSender::pushMessage(const Message &message) 
	{
		sendDeque.push_back(message);
	}

	// Get number of messages queued to be transmitted.
	int NetworkMessageSender::getNMessages() 
	{
		return sendDeque.size();
	}
	
	// Transmits queued messages.
	void NetworkMessageSender::transmit() 
	{
		int result;
		
		while (!sendDeque.empty())
		{
			// Retrieve message from queue
			Message message = sendDeque.front();
			sendDeque.pop_front();
			
			//printf("sending message: type: %d, time: %d, size: %d\n", message.type, message.time, message.size);
			
			//type = SDL_SwapBE32(type);
			
			// Send message on socket
			result = SDLNet_TCP_Send(socket, &(message.type), sizeof(message.type));
			if (result < sizeof(message.type))
			{ 
				printf("SDLNet_TCP_Send: %s (sending message.typec)\n", SDLNet_GetError());
				return; 
			}
			
			// Send time on socket
			result = SDLNet_TCP_Send(socket, &(message.time), sizeof(message.time));
			if (result < sizeof(message.time))
			{ 
				printf("SDLNet_TCP_Send: %s (sending message.time)\n", SDLNet_GetError());
				return; 
			}
			
			
			//len = SDL_SwapBE32(len);
			
			// Send length of message.data
			result = SDLNet_TCP_Send(socket, &(message.size), sizeof(message.size));
			if (result < sizeof(message.size))
			{ 
				printf("SDLNet_TCP_Send: %s (sending message.size)\n", SDLNet_GetError());
				return; 
			}

			//len = SDL_SwapBE32(len);

			// Send message.data
			result = SDLNet_TCP_Send(socket, message.data, message.size);
			if (result < message.size)
			{ 
				printf("SDLNet_TCP_Send: %s (sending message.data)\n", SDLNet_GetError());
				return; 
			}
			
			delete message.data;

			//if (result < len)
			//	printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
		}
	}

	//-----------------------------------------------------------
	// NETWORK MESSAGE RECIEVER
	//-----------------------------------------------------------
	bool NetworkMessageReciever::hasMessageOnQueue()
	{
		retrieve();

		return MessageReciever::hasMessageOnQueue();		
	}


	int NetworkMessageReciever::readData(void *data, int len)
	{
		int numready = SDLNet_CheckSockets(set, 0);
		if (numready == -1)
		{
			printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
			return 0;
		}

		if (numready && SDLNet_SocketReady(socket))
		{
			int result = SDLNet_TCP_Recv(socket, data, len);

			if (result < len)
			{ 
				printf("SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
				return 0;
			}
			
			return result;
		}		
		
		return 0;	
	}

	void NetworkMessageReciever::retrieve()
	{
		while (true)
		{
			switch (retrieveMessagePhase)
			{
			// Read type
			case 0:
			{
				if (readData(&retrieveType, sizeof(retrieveType)) == 0)
					return;
					
				retrieveMessagePhase++;
				break;
			}
	
			// Read time
			case 1:
			{
				if (readData(&retrieveTime, sizeof(retrieveTime)) == 0)
					return;
					
				retrieveMessagePhase++;
				break;		
			}
				
			// Read length of data
			case 2:
			{
				if (readData(&retrieveSize, sizeof(retrieveSize)) == 0)
					return;
					
				retrieveMessagePhase++;
				break;
			}
							
			// Read data
			case 3:
			{
				// Allocate memory for message data
				char *data = new char[retrieveSize];

				if (readData(data, retrieveSize) == 0)
				{
					delete [] data;
					return;
				}

				Message message(retrieveType, retrieveSize, data);
				
				//printf("retrieving message: type: %d, size: %d @ %d\n", retrieveType, retrieveSize, SDL_GetTicks());
				
				putMessageToLagQueue(message);
				
				// reset messagePhase
				retrieveMessagePhase = 0;
				return;
			}

			default:
				assert(false);
			}
		}
	}
	
	void NetworkMessageReciever::setSocket(TCPsocket socket)
	{
		this->socket = socket;

		set = SDLNet_AllocSocketSet(1);
		if (!set)
		{
			printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
			SDLNet_Quit();
			SDL_Quit();
			exit(4); //most of the time this is a major error, but do what you want.
		}		

		if (SDLNet_TCP_AddSocket(set, socket) == -1)
		{
			printf("SDLNet_TCP_AddSocket: %s\n", SDLNet_GetError());
			SDLNet_Quit();
			SDL_Quit();
			exit(7);
		}

	}
	
	//-----------------------------------------------------------
	// SERVER
	//-----------------------------------------------------------
	void NetworkServer::start()
	{
		IPaddress ip;
		size_t port = 12333;

		// Create a server type IPaddress
		if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
		{
			printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			exit(3);
		}

		// open the server socket
		socket = SDLNet_TCP_Open(&ip);
		if(!socket)
		{
			printf("SDLNet_TCP_Open (server): %s\n", SDLNet_GetError());
			exit(4);
		}
	}

	NetworkServerClient* NetworkServer::checkForNewClient()
	{
		TCPsocket serverClientSocket = 0;
		IPaddress *remoteip;
		createSocketSet();
		size_t numReady = SDLNet_CheckSockets(set, (size_t)1);

		if (numReady == -1)
		{
			// error
			return 0;
		}
		
		if (numReady == 0)
		{
			return 0;
		}
		
		//char message[1024];
		//int len;
		//Uint32 ipaddr;

		// check for new connection from a client
		if (SDLNet_SocketReady(socket))
		{
			// try to accept a connection
			serverClientSocket = SDLNet_TCP_Accept(socket);

			if(!serverClientSocket)
			{ // no connection accepted
				printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
				SDL_Delay(100); //sleep 1/10th of a second
				return 0;
			}
			
			// get the clients IP and port number
			remoteip = SDLNet_TCP_GetPeerAddress(serverClientSocket);

			if(!remoteip)
			{
				printf("SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
				return 0;
			}

			// print out the clients IP and port number
			Uint32 ipaddr = SDL_SwapBE32(remoteip->host);
			printf("Accepted a connection from %d.%d.%d.%d port %hu\n",
				ipaddr>>24,
				(ipaddr>>16)&0xff,
				(ipaddr>>8)&0xff,
				ipaddr&0xff,
				remoteip->port);
			
			NetworkServerClient *serverClient = new NetworkServerClient();
			serverClient->sender.setSocket(serverClientSocket);
			serverClient->reciever.setSocket(serverClientSocket);
			serverClient->socket = serverClientSocket;
			
			clients.push_back(serverClient);
			numberOfClients++;
			
			return serverClient;
		}
		
		return 0;
	}

	void NetworkServer::close()
	{
		Clients::iterator it = clients.begin();
		Clients::iterator end = clients.end();
		
		for ( ; it != end; ++it)
		{
			NetworkServerClient* serverClient = *it;
		
			delete serverClient;
		}
		
		SDLNet_FreeSocketSet(set);
	}
		
	void NetworkServer::createSocketSet()
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
		
		SDLNet_TCP_AddSocket(set, socket);
		
		for (size_t i = 0; i < numberOfClients; i++)
		{
			SDLNet_TCP_AddSocket(set, clients[i]->socket);
		}
	}


	//-----------------------------------------------------------
	// CLIENT
	//-----------------------------------------------------------
	bool NetworkClient::openConnection(std::string &host)
	{
		IPaddress ip;
		//char message[1024];
		//int len;
		Uint16 port = 12333;

		// Resolve the argument into an IPaddress type
		if (SDLNet_ResolveHost(&ip, host.c_str(), port) == -1)
		{
			printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
			return false;
		}

		// open the server socket
		socket = SDLNet_TCP_Open(&ip);
		if(!socket)
		{
			printf("SDLNet_TCP_Open (client): %s\n",SDLNet_GetError());
			return false;
		}
	
		sender.setSocket(socket);
		reciever.setSocket(socket);
		
		return true;
	}
	
	void NetworkClient::close()
	{
		SDLNet_TCP_Close(socket);
	}
		
};