#include "Server.h"

#include "NetworkConnection.h"
#include "Game.h"

#include "common.h"


namespace Planet
{

	Server::Server() {}




	Server::~Server() {}



	void Server::run(bool &runningServer, int numberOfClients)
	{
		getTimeHandler()->reset();
		
		NetworkServer networkServer;
		networkServer.start();
		
		NetworkServerClient *serverClient = 0;
		
		int clientsConnected = 0;

		// Wait for clients to connect
		while (runningServer)
		{
			if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: Waiting for clients to connect (clientsConnected = %d)\n", clientsConnected);
		
			serverClient = networkServer.checkForNewClient();

			if (serverClient != 0)
				clientsConnected++;
				
			if (clientsConnected == numberOfClients)
				break;
		
			SDL_Delay(20);
		}
		
		int clientsInitialized = 0;
		
		// Initialize clients
		while (runningServer)
		{
			if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: Waiting for clients to initialize, clientsInitialized: %d\n", clientsInitialized);

			Clients clients = networkServer.getClients();
		
			for (Clients::iterator it = clients.begin() ; it != clients.end(); ++it)
			{
				NetworkServerClient* serverClient = *it;

				MessageSender *sender = &(serverClient->sender);
				MessageReciever *reciever = &(serverClient->reciever);

				if (!serverClient->connected && clientConnected(sender, reciever))
				{
					serverClient->connected = true;
					clientsInitialized++;
				}
			}
			
				
			if (clientsInitialized == numberOfClients)
				break;
				
			SDL_Delay(5);
		}

		// server.startGame();

		while (runningServer)
		{
			printf("Running Server.\n");
			SDL_Delay(1000);
		}
	}


	bool Server::clientConnected(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		// Temp Link used to simplify sending and retrieving messages during this connection phase.
		Link link(messageSender, messageReciever);

		link.retrieve(getTimeHandler()->getTime());

		if (link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == INIT_CLIENT)
			{
				// retrieve InitClient message from client
				InitClient *initClient = link.getPoppedData<InitClient>();
				Color color = initClient->color;

				// add player to server
				PlayerId playerId = addClient(color, messageSender, messageReciever);

				Pos startPos(200.0f + playerId * 50.0f, 200.0f);
//%				addPlayerObj(playerId, startPos);

				// send WelcomeClient with playerId to client
				WelcomeClient welcomeClient = WelcomeClient(playerId);
				link.pushMessage(welcomeClient, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				link.transmit();

			}
			else if (messageType == SYNC_PING)
			{
				// retrieve SyncPing from client
				SyncPing *syncPing = link.getPoppedData<SyncPing>();
				PlayerId playerId = syncPing->playerId;
				int pingSendTime = syncPing->pingSendTime;

				// send SyncPong to client
				SyncPong syncPong(getTimeHandler()->getTime(), pingSendTime);
				link.pushMessage(syncPong, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				link.transmit();

				return true;
			}
			else
			{
				assert(false);
			}
		}
		
		return false; // no client did try to connect
	}

	PlayerId Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	{
		PlayerId playerId = getIdGenerator()->generatePlayerId();
		players.add(playerId, new ServerPlayer(color, messageSender, messageReciever));
		return playerId;
	}
	
};
