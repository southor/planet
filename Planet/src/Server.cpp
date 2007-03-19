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

	void Server::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	{
		planet.addPlayerObj(playerId, playerPos);
	}

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
		printf("generated id: %d\n", playerId);
		players.add(playerId, new ServerPlayer(color, messageSender, messageReciever));
		return playerId;
	}

	void Server::logic()
	{
		const int tick = getTimeHandler()->getTick();
		const int time = getTimeHandler()->getTime();
			
		if (tick == 0)
		{
			int tickFromTime = 10 + getTimeHandler()->getTickFromTime();
			getTimeHandler()->setTick(tickFromTime);
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
						
						//if (userCmd->shootAction == UserCmd::CONTINUE_SHOOTING)
						//{
						//	// check that privious really is START_SHOOTING or CONTINUE_SHOOTING
						//	UserCmd preUserCmd;
						//	player.getUserCmd(preUserCmd, getTimeHandler()->getTick() - 1);
						//	if (preUserCmd.shootAction == UserCmd::NOT_SHOOTING)
						//	{
						//		userCmd->shootAction = UserCmd::START_SHOOTING;
						//	}
						//}

						std::cout << "diff = " << getTimeHandler()->getTick() - player->link.getPoppedTick() << std::endl;
						assert(userCmd->isConsistent(player->link.getPoppedTick()));
						player->setUserCmd(*userCmd, player->link.getPoppedTick());
					}

					//else if (messageType == SHOOT_CMD)
					//{
					//	printf("SERVER: handling shoot_cmd @ %d\n", getTimeHandler()->getTime());

					//	// player shoots
					//	ShootCmd *shootCmd = player.link.getPoppedData<ShootCmd>();					
					//	GameObjId projectileId = worldModel.playerShoot(shootCmd->playerId, shootCmd->weapon);
					//	Projectile *projectile = (worldModel.getProjectiles())[projectileId];
					//	
					//	// send projectile to all clients
					//	AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
					//								projectile->getAngle().getFloat(), projectile->getShooterId());
					//	pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
					//}
				}

				PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];
				playerObj->updateToTickData(getTimeHandler()->getTick());
				UserCmd userCmd;
				player->getUserCmd(userCmd, getTimeHandler()->getTick());
				userCmd.isConsistent(getTimeHandler()->getTick());
				playerObj->setUserCmd(&userCmd);

				// shooting
				//if (userCmd.nShots > 0)
				//{
				//	std::cout << getTimeHandler()->getTick() << "  server: nShots: " << userCmd.nShots << std::endl;
				//}
				//std::vector<GameObjId> shots;
				planet.handlePlayerShooting(playerId, players);
				//for(size_t i=0; i<shots.size(); ++i)
				//{
				//	GameObjId projectileId = shots[i];
 				//	Projectile *projectile = (worldModel.getProjectiles())[projectileId];
				//	
				//	// send projectile to all clients
				//	AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
				//								projectile->getAngle().getFloat(), projectile->getShooterId(), projectile->getShootTick(), projectile->getObjLag());
				//	pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				//}
			}		

			// handle projectile hits
			planet.performProjectileHits(players);
			
			// update movements of objects, from currentTick to currentTick + 1
			planet.updateProjectileMovements();
			planet.updatePlayerObjMovements();
			
			// Update next shoot tick for currentTick + 1
			Planet::PlayerObjs::Iterator playerObjsIt = planet.getPlayerObjs().begin();
			Planet::PlayerObjs::Iterator playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{				
				playerObjsIt->second->updateNextShootTick(getTimeHandler()->getTick());
			}
			
			// next tick!!
			getTimeHandler()->nextTick();
			lastUpdateTime = time;

			// Send playerObj updates and store state to history, also send tick0Time
			playerObjsIt = planet.getPlayerObjs().begin();
			playerObjsEnd = planet.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				//GameObjId playerObjId = playerObjsIt->first;
				PlayerId playerId = playerObjsIt->first;
				PlayerObj *playerObj = playerObjsIt->second;
				
				//UpdatePlayerObj updatePlayerObj(playerObjId, playerObj->pos, playerObj->angle);
				UpdatePlayerObj updatePlayerObj(playerId, playerObj->getPos(), playerObj->getAimPos(), playerObj->getNextShootTick(), playerObj->getAmmoSupply());

				pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());

				//playerObj->isConsistent();
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
			}

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
	
};
