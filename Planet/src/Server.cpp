#include "Server.h"

#include "NetworkConnection.h"
#include "Game.h"

#include "common.h"


namespace Planet
{
	const double Server::PREDICTION_AMOUNT_MODIFIER = 1.1;
	const int Server::PREDICTION_AMOUNT_ADD_TIME = 10;

	Server::Server() : ServerGlobalAccess(&serverGlobalObj), lastUpdateTime(0), planet(&serverGlobalObj)
	{}

	Server::~Server()
	{
		DeleteSecond<ClientPlayers::Pair> deleteSecond;
		ForEach(players.begin(), players.end(), deleteSecond);
	}

	void Server::init()
	{
		planet.init(currentMap);
	}

	void Server::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	{
		planet.addPlayerObj(playerId, playerPos);
	}

	void Server::run(bool &runningServer, int numberOfClients, std::string map)
	{
		getTimeHandler()->reset();
		setMap(map);

		init();

		NetworkServer networkServer;
		networkServer.start();
		
		NetworkServerClient *serverClient = 0;
		
		int clientsConnected = 0;

		if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: Waiting for clients to connect...\n");

		// Wait for clients to connect
		while (runningServer)
		{
			serverClient = networkServer.checkForNewClient();

			if (serverClient != 0)
			{
				clientsConnected++;
				if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: clientsConnected = %d\n", clientsConnected);
			}
				
			if (clientsConnected == numberOfClients)
				break;
		
			SDL_Delay(20);
		}
		
		int clientsInitialized = 0;

		if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: Waiting for clients to initialize..\n");
		
		// Initialize clients
		while (runningServer)
		{
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
					if (SERVER_PRINT_NETWORK_DEBUG) printf("SERVER: clientsInitialized: %d\n", clientsInitialized);
				}
			}
			
				
			if (clientsInitialized == numberOfClients)
				break;
				
			SDL_Delay(5);
		}

		startGame();

		while (runningServer)
		{
			for (int i = 0; i < 3; i++)
				logic();
				
			SDL_Delay(TimeHandler::TICK_DELTA_TIME / 8);			
		}
	}

	void Server::startGame()
	{
		//getTimeHandler()->reset();
		
		Planet::PlayerObjs::Iterator playerObjsIt = planet.getPlayerObjs().begin();
		Planet::PlayerObjs::Iterator playerObjsEnd = planet.getPlayerObjs().end();
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{
			PlayerId playerId = playerObjsIt->first;
			PlayerObj *playerObj = playerObjsIt->second;

			//Color color(static_cast<float>(playerId % 2), 1.0f-static_cast<float>(playerId % 2), 0.0f); // the correct color should be retrieved from Player

			AddPlayerObj addPlayerObj(playerId, players[playerId]->color, playerObj->getPos(), playerObj->getAimPos());

			pushMessageToAll(players, addPlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		}

		transmitAll(players);
		
	
		//// TODO Send the hole worldmodel to clients, all players and everything
		//ServerPlayers::Iterator playersIt;
		//for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		//{			
		//	const ServerPlayer *player = playersIt->second;

		//	WorldModel::Obstacles::Iterator obstaclesIt = worldModel.getObstacles().begin();
		//	WorldModel::Obstacles::Iterator obstaclesEnd = worldModel.getObstacles().end();
		//	for(; obstaclesIt != obstaclesEnd; ++obstaclesIt)
		//	{
		//		GameObjId obstacleId = obstaclesIt->first;
		//		Obstacle *obstacle = obstaclesIt->second;
		//		
		//		AddObstacle addObstacle(obstacleId, *obstacle);
		//		player->link.pushMessage(addObstacle, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		//	}

		//	player->link.transmit();
		//}
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

				Pos startPos(0.0f, 0.0f, 6.0f);
				addPlayerObj(playerId, startPos);

				// send WelcomeClient with playerId to client
				WelcomeClient welcomeClient = WelcomeClient(playerId, currentMap);
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
		printf("generated id: %d\n", playerId);
		players.add(playerId, new ServerPlayer(playerId, color, messageSender, messageReciever));
		return playerId;
	}

	void Server::logic()
	{
		const int tick = getTimeHandler()->getTick();
		const int time = getTimeHandler()->getTime();
			

		Planet::PlayerObjs::Iterator playerObjsIt;
		Planet::PlayerObjs::Iterator playerObjsEnd;

		if (tick == 0)
		{
			int tickFromTime = 10 + getTimeHandler()->getTickFromTime();
			getTimeHandler()->setTick(tickFromTime);
			
			
			// Update next shoot tick for currentTick
			
			playerObjsIt = planet.getPlayerObjs().begin();
			playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{				
				playerObjsIt->second->tickInit(playerObjsIt->first, getTimeHandler()->getTick());
				//playerObjsIt->second->updateNextShootTick(getTimeHandler()->getTick() - 1);
				assert(playerObjsIt->second->isConsistent(getTimeHandler()->getTick()));
			}
			
			printf("start tick: %d, time: %d\n", tickFromTime, getTimeHandler()->getTime());
		}

		bool waitingForClients = false;
		int latestTick = 10000000; // used for debugging

		// check if current tick is recieved from all clients, otherwise set waitingForClients to true
		ServerPlayers::Iterator playersIt;
		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		{
			PlayerId playerId = playersIt->first;
			ServerPlayer *player(playersIt->second);
			
			player->link.retrieve(getTimeHandler()->getTime());

			// set waitingForClients to true if player doesn't have current tick
			waitingForClients = waitingForClients || (player->link.getLatestTick() < tick);

			//printf("playerId: %d, latestTick: %d\n", playerId, player.link.getLatestTick());

			if (player->link.getLatestTick() < latestTick) // used for debugging
				latestTick = player->link.getLatestTick(); // used for debugging

			// Check tick timeout
			if (getTimeHandler()->getTickFromTimeWithTimeout() >= tick)      //if (time > lastUpdateTime + 100) //ServerTimeHandler::TICK_DELTA_TIME + ServerTimeHandler::WAIT_FOR_TICK_TIMEOUT)
			{
				if (SERVER_PRINT_NETWORK_DEBUG) printf("#################### TIMEOUT ######################\n");
				waitingForClients = false;
				break; // exit for loop
			}
		}

		if (!waitingForClients)
		{
			if (SERVER_PRINT_NETWORK_DEBUG) printf("run tick: %d, tickFromTime: %d, tickWithTO: %d, latest: %d @ %d\n", tick, getTimeHandler()->getTickFromTime(), getTimeHandler()->getTickFromTimeWithTimeout(), latestTick, time);

			if ((static_cast<int>(getTimeHandler()->getTick()) % (2000/TimeHandler::TICK_DELTA_TIME)) == 0)
			{
				debugPrintState();
			}

			lastUpdateTime = time;

			planet.isConsistent();

			// Read messages from clients
			//ServerPlayers::Iterator playersIt;
			for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
			{
				PlayerId playerId = playersIt->first;
				ServerPlayer *player(playersIt->second);

				player->link.retrieve(getTimeHandler()->getTime());

				while (player->link.hasMessageOnQueueWithTick(tick))
				{
					int messageType = player->link.popMessage();
					if (messageType == USER_CMD)
					{
						UserCmd *userCmd = player->link.getPoppedData<UserCmd>();
						
						assert(userCmd->isConsistent(playerId, player->link.getPoppedTick()));
						player->setUserCmd(*userCmd, player->link.getPoppedTick());
					}
				}

				PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];
				assert(playerObj->isConsistent(getTimeHandler()->getTick()));
				UserCmd userCmd;
				player->getUserCmd(userCmd, getTimeHandler()->getTick());
				userCmd.isConsistent(getTimeHandler()->getTick());
				playerObj->setUserCmd(&userCmd);

				// All player object data for this tick has been set, send and store to history!
				UpdatePlayerObj updatePlayerObj(playerId, playerObj->getPos(), playerObj->getAimPos(), playerObj->getNextShootTick(), playerObj->getAmmoSupply());
				pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				assert(playerObj->isConsistent(getTimeHandler()->getTick()));
				playerObj->storeToTickData(getTimeHandler()->getTick());

				// Send tick0Time to client
				{
					Link &link = players[playerId]->link;
					double lag = tmax(static_cast<double>(link.getCurrentLag()), 0.0);
					int extraPredictionTime = static_cast<int>(lag * PREDICTION_AMOUNT_MODIFIER) + PREDICTION_AMOUNT_ADD_TIME;
					assert(extraPredictionTime >= 0);
					SetTick0Time tick0Time(-extraPredictionTime);
					
					link.pushMessage(tick0Time, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				}

				planet.handlePlayerShooting(playerId, players);

			}

			// transmit some messages
			transmitAll(players);

			// handle projectile hits, will update to history and back again without storing!!!
			planet.performProjectileHits(players);
			
			// update movements of objects, from currentTick to currentTick + 1
			planet.updateProjectileMovements();
			planet.updatePlayerObjMovements();
			
			// Update next shoot tick for currentTick + 1
			playerObjsIt = planet.getPlayerObjs().begin();
			playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{				
				playerObjsIt->second->updateNextShootTick(getTimeHandler()->getTick());
				assert(playerObjsIt->second->isConsistent(getTimeHandler()->getTick() + 1));
			}
			
			// next tick!!
			getTimeHandler()->nextTick();
			lastUpdateTime = time;

			// Send projectile updates
			Planet::Projectiles::Iterator projectilesIt = planet.getProjectiles().begin();
			Planet::Projectiles::Iterator projectilesEnd = planet.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{			
				Projectile *projectile = projectilesIt->second;
				
				UpdateProjectile updateProjectile(projectilesIt->first, projectile->getPos());
				
				pushMessageToAll(players, updateProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());

				//projectile->storeToTickData(getTimeHandler()->getTick());
			}

			transmitAll(players);
		}
	}

	void Server::debugPrintState()
	{
		std::cout << "Server tick: " << static_cast<int>(getTimeHandler()->getTick());
		std::cout << "     Number of projectiles: " << planet.getProjectiles().getSize() << std::endl;
	}
	
};
