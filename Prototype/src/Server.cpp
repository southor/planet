#include "Server.h"
#include "common.h"

//// resolving LNK error
//#include "WorldRenderer.cpp"

namespace Prototype
{

	const double Server::PREDICTION_AMOUNT_MODIFIER_DEFAULT = 1.1;
	const int Server::PREDICTION_AMOUNT_ADD_TIME_DEFAULT = 10;

	Server::Server() : worldRenderer(WorldRenderer::WORLD_CENTER), lastUpdateTime(0),
						ServerGlobalAccess(&serverGlobalObj), worldModel(&serverGlobalObj),
						requestRender(false)
	{
		if (configHandler.loadFile(CONFIG_FILENAME))
		{
			std::cout << "Server: using config file for game parameters" << std::endl;
		}
		else
		{
			std::cout << "* * * Server: Config file not found, using default values!!! * * *" << std::endl;
		}
		
		// outer walls
		static const float WALL_THICKNESS = 500.0f;
		Rectangle wall1(0.0f, -WALL_THICKNESS, WorldModel::WORLD_SIZE.x, WALL_THICKNESS);
		Rectangle wall2(0.0f, WorldModel::WORLD_SIZE.y, WorldModel::WORLD_SIZE.x, WALL_THICKNESS);
		Rectangle wall3(-WALL_THICKNESS, -WALL_THICKNESS, WALL_THICKNESS, WorldModel::WORLD_SIZE.y + WALL_THICKNESS*2.0f);
		Rectangle wall4(WorldModel::WORLD_SIZE.x, -WALL_THICKNESS, WALL_THICKNESS, WorldModel::WORLD_SIZE.y + WALL_THICKNESS*2.0f);
		worldModel.addObstacle(wall1);
		worldModel.addObstacle(wall2);
		worldModel.addObstacle(wall3);
		worldModel.addObstacle(wall4);

		Rectangle obstacleArea(50.0f, 70.0f, 150.0f, 80.0f);
		worldModel.addObstacle(obstacleArea);
		obstacleArea.pos.x = 300.0f;
		worldModel.addObstacle(obstacleArea);

		Pos respawnPos1(50.0f, 250.0f);
		Pos respawnPos2(250.0f, 200.0f);
		worldModel.addRespawnPos(respawnPos1);
		worldModel.addRespawnPos(respawnPos2);
	}

	Server::~Server()
	{
		ServerPlayers::Iterator it(players.begin());
		ServerPlayers::Iterator end(players.end());
		for(;it != end; ++it)
		{
			delete it->second;
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
		players.add(playerId, new ServerPlayer(playerId, color, messageSender, messageReciever));

		return playerId;
	}

	void Server::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	{
		worldModel.addPlayerObj(playerId, playerPos, configHandler.getIntValue("player_obj_health", PlayerObj::HEALTH_DEFAULT));
	}

	void Server::startGame()
	{
		WorldModel::PlayerObjs::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
		WorldModel::PlayerObjs::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{
			PlayerId playerId = playerObjsIt->first;
			PlayerObj *playerObj = playerObjsIt->second;

			AddPlayerObj addPlayerObj(playerId, players[playerId]->color, playerObj->pos);

			pushMessageToAll(players, addPlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		}
	
		ServerPlayers::Iterator playersIt;
		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		{			
			const ServerPlayer *player = playersIt->second;

			WorldModel::Obstacles::Iterator obstaclesIt = worldModel.getObstacles().begin();
			WorldModel::Obstacles::Iterator obstaclesEnd = worldModel.getObstacles().end();
			for(; obstaclesIt != obstaclesEnd; ++obstaclesIt)
			{
				GameObjId obstacleId = obstaclesIt->first;
				Obstacle *obstacle = obstaclesIt->second;
				
				AddObstacle addObstacle(obstacleId, *obstacle);
				player->link.pushMessage(addObstacle, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			player->link.transmit();
		}
	}

	void Server::logic()
	{
		const int tick = getTimeHandler()->getTick();
		const int time = getTimeHandler()->getTime();
		

		WorldModel::PlayerObjs::Iterator playerObjsIt;
		WorldModel::PlayerObjs::Iterator playerObjsEnd;

		if (tick == 0)
		{
			int tickFromTime = 10 + getTimeHandler()->getTickFromTime();
			getTimeHandler()->setTick(tickFromTime);

			// Update next shoot tick for currentTick
			
			playerObjsIt = worldModel.getPlayerObjs().begin();
			playerObjsEnd = worldModel.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{				
				playerObjsIt->second->tickInit(playerObjsIt->first, getTimeHandler()->getTick());
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

			if (player->link.getLatestTick() < latestTick) // used for debugging
				latestTick = player->link.getLatestTick(); // used for debugging

			// Check tick timeout
			if (getTimeHandler()->getTickFromTimeWithTimeout() >= tick)
			{
				if (configHandler.getIntValue("server_print_timeout_debug", SERVER_PRINT_TIMEOUT_DEBUG_DEFAULT) == 1) printf("#################### TIMEOUT ######################\n");
				waitingForClients = false;

				break; // exit for loop
			}
		}

		if (!waitingForClients)
		{
			if (configHandler.getIntValue("server_print_tick_debug", SERVER_PRINT_TICK_DEBUG_DEFAULT) == 1) printf("run tick: %d, tickFromTime: %d, tickWithTO: %d, latest: %d @ %d\n", tick, getTimeHandler()->getTickFromTime(), getTimeHandler()->getTickFromTimeWithTimeout(), latestTick, time);

			if ((static_cast<int>(getTimeHandler()->getTick()) % (3000/TimeHandler::TICK_DELTA_TIME)) == 0)
			{
				debugPrintState();
			}

			lastUpdateTime = time;

			assert(worldModel.isConsistent());

			// Read messages from clients
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

				PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
				assert(playerObj->isConsistent(getTimeHandler()->getTick()));
				UserCmd userCmd;
				player->getUserCmd(userCmd, getTimeHandler()->getTick());
				userCmd.isConsistent(getTimeHandler()->getTick());
				
				playerObj->setUserCmd(&userCmd);

				// All player object data for this tick has been set, now send and store to history!
				UpdatePlayerObj updatePlayerObj(playerId, playerObj->pos, playerObj->angle, playerObj->getNextShootTick(), playerObj->getAmmo());
				pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				assert(playerObj->isConsistent(getTimeHandler()->getTick()));
				playerObj->storeToTickData(getTimeHandler()->getTick());

				// Send tick0Time to client
				{
					Link &link = players[playerId]->link;
					double lag = tmax(static_cast<double>(link.getCurrentLag()), 0.0);
					int extraPredictionTime = static_cast<int>(lag * configHandler.getDoubleValue("prediction_amount_modifier", PREDICTION_AMOUNT_MODIFIER_DEFAULT)) +
																		configHandler.getIntValue("prediction_amount_add_time", PREDICTION_AMOUNT_ADD_TIME_DEFAULT);
					SetTick0Time tick0Time(-extraPredictionTime);
					link.pushMessage(tick0Time, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				}
				
				worldModel.handlePlayerShooting(playerId, players);
			}

			// transmit some messages
			transmitAll(players);

			// handle projectile hits, will update to history!!!
			worldModel.performProjectileHits(players, &worldRenderer);

			// update movements of objects, from currentTick to currentTick + 1
			worldModel.updateProjectileMovements();
			worldModel.updatePlayerObjMovements();
			
			// Update next shoot tick for currentTick + 1
			playerObjsIt = worldModel.getPlayerObjs().begin();
			playerObjsEnd = worldModel.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{				
				playerObjsIt->second->updateNextShootTick(getTimeHandler()->getTick());
				assert(playerObjsIt->second->isConsistent(getTimeHandler()->getTick() + 1));
			}
			
			// next tick!!
			getTimeHandler()->nextTick();
			lastUpdateTime = time;

			// Send projectile updates
			WorldModel::Projectiles::Iterator projectilesIt = worldModel.getProjectiles().begin();
			WorldModel::Projectiles::Iterator projectilesEnd = worldModel.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{			
				Projectile *projectile = projectilesIt->second;
				
				UpdateProjectile updateProjectile(projectilesIt->first, projectile->getPos());
				
				pushMessageToAll(players, updateProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			transmitAll(players);

			requestRender = true;
		}
	}

	void Server::render()
	{
		worldRenderer.setupProjection();			
		PlayerId playerIdFollow(1);
		if (worldModel.getPlayerObjs().exists(playerIdFollow))
		{
			worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerIdFollow]); //, static_cast<Tickf>(getTimeHandler()->getTick()));
		}
		requestRender = false;
	}

	void Server::debugPrintState()
	{
		std::cout << "Server tick: " << static_cast<int>(getTimeHandler()->getTick());
		std::cout << "     Number of projectiles: " << worldModel.getProjectiles().getSize() << std::endl;
	}
	
};
