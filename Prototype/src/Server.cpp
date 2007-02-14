#include "Server.h"
#include "common.h"

namespace Prototype
{

	Server::Server() : lastUpdateTime(0.0f), ServerGlobalAccess(&serverGlobalObj), worldModel(&serverGlobalObj)
	{
		// outer walls
		static const int WALL_THICKNESS = 500.0f;
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

	bool Server::clientConnected(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		// Temp Link used to simplify sending and retrieving messages during this connection phase.
		Link link(messageSender, messageReciever);

		if (link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == INIT_CLIENT)
			{
				// retrieve InitClient message from client
				InitClient *initClient = link.getPoppedData<InitClient>();
				Color color = initClient->color;

				// add player to server
				size_t playerId = addClient(color, messageSender, messageReciever);

				Pos startPos(200.0f + playerId * 50.0f, 200.0f);
				addPlayerObj(playerId, startPos);

				// send WelcomeClient with playerId to client
				WelcomeClient welcomeClient = WelcomeClient(playerId);
				link.pushMessage(welcomeClient, getTimeHandler()->getTime());
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

	// TODO: MAKE PRIVATE
	size_t Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	{

		size_t playerId = players.findFreeId();
		players.add(playerId, ServerPlayer(color, messageSender, messageReciever));
		return playerId;
		
		//ServerClient client(messageSender, messageReciever);

		//addClient(client);
	}

	// TODO: MAKE PRIVATE
	void Server::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		worldModel.addPlayerObj(playerId, playerPos);
		//size_t playerObjId = worldModel.addPlayerObj(playerId, playerPos);
		//players[playerId].playerObjId = playerObjId;
		//return playerObjId;
	}

	void Server::startGame()
	{
		getTimeHandler()->reset();
		
		WorldModel::PlayerObjContainer::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
		WorldModel::PlayerObjContainer::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{
			size_t playerId = playerObjsIt->first;
			PlayerObj *playerObj = playerObjsIt->second;

			Color color(static_cast<float>(playerId), 1.0f-static_cast<float>(playerId), 0.0f); // the correct color should be retrieved from Player

			AddPlayerObj addPlayerObj(playerId, color, playerObj->pos);

			pushMessageToAll(players, addPlayerObj, getTimeHandler()->getTime());
		}
	
		// TODO Send the hole worldmodel to clients, all players and everything
		ServerPlayers::Iterator playersIt;
		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		{			
			const ServerPlayer &player = playersIt->second;

			WorldModel::ObstacleContainer::Iterator obstaclesIt = worldModel.getObstacles().begin();
			WorldModel::ObstacleContainer::Iterator obstaclesEnd = worldModel.getObstacles().end();
			for(; obstaclesIt != obstaclesEnd; ++obstaclesIt)
			{
				size_t obstacleId = obstaclesIt->first;
				Obstacle *obstacle = obstaclesIt->second;
				
				AddObstacle addObstacle(obstacleId, *obstacle);
				player.link.pushMessage(addObstacle, getTimeHandler()->getTime());
			}

			player.link.transmit();
		}
	}

	//size_t Server::addObstacle(const Rectangle &obstacleArea)
	//{
	//	return worldModel.addObstacle(obstacleArea);
	//}
	
	void Server::logic()
	{
		int tick = getTimeHandler()->getTick();
		int time = getTimeHandler()->getTime();
		bool waitingForClients = false;

		// check if current tick is recieved from all clients, otherwise set waitingForClients to true
		{
			ServerPlayers::Iterator playersIt;
			for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
			{
				size_t playerId = playersIt->first;
				ServerPlayer player(playersIt->second);
				
				if (player.link.hasMessageOnQueueWithTick(tick))
				{	
					// if message with old tick then discard
					if (player.link.getTickOfMessageOnQueue() < tick)
					{
						//player.link.popMessage();
					}
					else
					{
						player.latestTick = tick;
					}
				}
				
				// set waitingForClients to true if player doesn't have current tick
				waitingForClients = waitingForClients || (player.latestTick != tick);

				// Check tick timeout
				if (time > lastUpdateTime + ServerTimeHandler::TICK_DELTA_TIME + ServerTimeHandler::WAIT_FOR_TICK_TIMEOUT)
				{
					waitingForClients = false;
					break; // exit for loop
				}
			}
		}

		if (!waitingForClients)
		{
			float deltaTime = ServerTimeHandler::TICK_DELTA_TIME;
			lastUpdateTime = time;

			worldModel.isConsistent();


			// Read messages from clients
			ServerPlayers::Iterator playersIt;
			for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
			{			
				size_t playerId = playersIt->first;
				ServerPlayer player(playersIt->second);

				while (player.link.hasMessageOnQueueWithTick(tick))
				{
					int messageType = player.link.popMessage();
					if (messageType == USER_CMD)
					{
						UserCmd *userCmd = player.link.getPoppedData<UserCmd>();

						//PlayerObj *playerObj = (worldModel.getPlayerObjs())[player.playerObjId];
						PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
						playerObj->movingForward = userCmd->cmdUp;
						playerObj->movingBackward = userCmd->cmdDown;
						playerObj->strafingLeft = userCmd->cmdLeft;
						playerObj->strafingRight = userCmd->cmdRight;
						playerObj->angle = userCmd->viewangle;
					}
					else if (messageType == SHOOT_CMD)
					{
						// player shoots
						ShootCmd *shootCmd = player.link.getPoppedData<ShootCmd>();					
						size_t projectileId = worldModel.playerShoot(shootCmd->playerId, shootCmd->weapon);
						Projectile *projectile = (worldModel.getProjectiles())[projectileId];
						
						// send projectile to all clients
						AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(), projectile->getAngle(), projectile->getShooterId());
						pushMessageToAll(players, addProjectile, getTimeHandler()->getTime());
					}
				}
			}		

			// update movements of objects
			worldModel.updatePlayerObjMovements(deltaTime);
			worldModel.updateProjectileMovements(deltaTime, players);

			// Send playerObj updates
			WorldModel::PlayerObjContainer::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
			WorldModel::PlayerObjContainer::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				//size_t playerObjId = playerObjsIt->first;
				size_t playerId = playerObjsIt->first;
				PlayerObj *playerObj = playerObjsIt->second;
				//UpdatePlayerObj updatePlayerObj(playerObjId, playerObj->pos, playerObj->angle);
				UpdatePlayerObj updatePlayerObj(playerId, playerObj->pos, playerObj->angle);

				pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime());
			}

			// Send projectile updates
			WorldModel::ProjectileContainer::Iterator projectilesIt = worldModel.getProjectiles().begin();
			WorldModel::ProjectileContainer::Iterator projectilesEnd = worldModel.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{			
				UpdateProjectile updateProjectile(projectilesIt->first, projectilesIt->second->getPos());
				
				pushMessageToAll(players, updateProjectile, getTimeHandler()->getTime());
			}

			transmitAll(players);
			
			getTimeHandler()->nextTick();
		}
	}
	
};

