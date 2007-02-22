#include "Server.h"
#include "common.h"

namespace Prototype
{

	Server::Server() : lastUpdateTime(0), ServerGlobalAccess(&serverGlobalObj), worldModel(&serverGlobalObj)
	{
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
	PlayerId Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	{

		//PlayerId playerId = players.findFreeId();
		PlayerId playerId = getIdGenerator()->generatePlayerId();
		players.add(playerId, ServerPlayer(color, messageSender, messageReciever));
		return playerId;
		
		//ServerClient client(messageSender, messageReciever);

		//addClient(client);
	}

	// TODO: MAKE PRIVATE
	void Server::addPlayerObj(PlayerId playerId, const Pos &playerPos)
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
			PlayerId playerId = playerObjsIt->first;
			PlayerObj *playerObj = playerObjsIt->second;

			Color color(static_cast<float>(playerId % 2), 1.0f-static_cast<float>(playerId % 2), 0.0f); // the correct color should be retrieved from Player

			AddPlayerObj addPlayerObj(playerId, color, playerObj->pos);

			pushMessageToAll(players, addPlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
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
				GameObjId obstacleId = obstaclesIt->first;
				Obstacle *obstacle = obstaclesIt->second;
				
				AddObstacle addObstacle(obstacleId, *obstacle);
				player.link.pushMessage(addObstacle, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			player.link.transmit();
		}
	}

	void Server::logic()
	{
		int tick = getTimeHandler()->getTick();
		int time = getTimeHandler()->getTime();
		
		if (tick == 0)
			getTimeHandler()->setTickStartTime(time);

		bool waitingForClients = false;

		//printf("tick: %d, tickWithTimeout: %d\n", tick, getTimeHandler()->getTickWithTimeout());


		// check if current tick is recieved from all clients, otherwise set waitingForClients to true
		{
			ServerPlayers::Iterator playersIt;
			for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
			{
				PlayerId playerId = playersIt->first;
				ServerPlayer player(playersIt->second);
				
				player.link.retrieve(getTimeHandler()->getTime());

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
				waitingForClients = true;
				// Check tick timeout

				//if (getTimeHandler()->getTickWithTimeout() > tick)
				if (time > lastUpdateTime + 100) //ServerTimeHandler::TICK_DELTA_TIME + ServerTimeHandler::WAIT_FOR_TICK_TIMEOUT)
				{
					waitingForClients = false;
					break; // exit for loop
				}
			}
		}

		if (!waitingForClients)
		{
			int deltaTime = ServerTimeHandler::TICK_DELTA_TIME;
			float deltaTimef = static_cast<float>(deltaTime);
			lastUpdateTime = time;

			worldModel.isConsistent();


			// Read messages from clients
			ServerPlayers::Iterator playersIt;
			for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
			{			
				PlayerId playerId = playersIt->first;
				ServerPlayer player(playersIt->second);

				player.link.retrieve(getTimeHandler()->getTime());

				while (player.link.hasMessageOnQueueWithTick(tick))
				{
					int messageType = player.link.popMessage();
					if (messageType == USER_CMD)
					{
						printf("SERVER: handling user_cmd @ %d\n", getTimeHandler()->getTime());
						
						UserCmd *userCmd = player.link.getPoppedData<UserCmd>();
						StateCmds stateCmds(userCmd->stateCmds);
						
						PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
						playerObj->movingForward = stateCmds.getCurrentState(Cmds::FORWARD);
						playerObj->movingBackward = stateCmds.getCurrentState(Cmds::BACKWARD);
						playerObj->strafingLeft = stateCmds.getCurrentState(Cmds::LEFT);
						playerObj->strafingRight = stateCmds.getCurrentState(Cmds::RIGHT);
						playerObj->angle = userCmd->aimangle;
					}
					else if (messageType == SHOOT_CMD)
					{
						printf("SERVER: handling shoot_cmd @ %d\n", getTimeHandler()->getTime());

						// player shoots
						ShootCmd *shootCmd = player.link.getPoppedData<ShootCmd>();					
						GameObjId projectileId = worldModel.playerShoot(shootCmd->playerId, shootCmd->weapon);
						Projectile *projectile = (worldModel.getProjectiles())[projectileId];
						
						// send projectile to all clients
						AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(), projectile->getAngle(), projectile->getShooterId());
						pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
					}
				}
			}		

			// update movements of objects
			worldModel.updatePlayerObjMovements(deltaTimef);
			worldModel.updateProjectileMovements(deltaTimef, players);

			// Send playerObj updates
			WorldModel::PlayerObjContainer::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
			WorldModel::PlayerObjContainer::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				//GameObjId playerObjId = playerObjsIt->first;
				PlayerId playerId = playerObjsIt->first;
				PlayerObj *playerObj = playerObjsIt->second;
				//UpdatePlayerObj updatePlayerObj(playerObjId, playerObj->pos, playerObj->angle);
				UpdatePlayerObj updatePlayerObj(playerId, playerObj->pos, playerObj->angle);

				pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			// Send projectile updates
			WorldModel::ProjectileContainer::Iterator projectilesIt = worldModel.getProjectiles().begin();
			WorldModel::ProjectileContainer::Iterator projectilesEnd = worldModel.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{			
				UpdateProjectile updateProjectile(projectilesIt->first, projectilesIt->second->getPos());
				
				pushMessageToAll(players, updateProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			transmitAll(players);
			
			getTimeHandler()->nextTick();
			lastUpdateTime = time;
		}
	}
	
};

