#include "Server.h"
#include "common.h"

namespace Prototype
{

	Server::Server()
	{
		Rectangle obstacleArea(50.0f, 70.0f, 150.0f, 80.0f);
		worldModel.addObstacle(obstacleArea);
		obstacleArea.pos.x = 300.0f;
		worldModel.addObstacle(obstacleArea);
	}

	void Server::clientConnected(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		Link link = Link(messageSender, messageReciever);

		// TODO: wait for this message to arive
		if (link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == INIT_CLIENT)
			{
				// retrieve InitClient message from client
				InitClient *initClient = link.getPoppedInitClient();
				Color color = initClient->color;
				
				
				// add player to server
				size_t playerId = addClient(color, link);  // link will get copied here, problem?

				Pos startPos(200.0f + playerId * 50.0f, 200.0f);
				addPlayerObj(playerId, startPos);

				// send WelcomeClient with playerId to client
				WelcomeClient welcomeClient = WelcomeClient(playerId);
				link.pushMessage(welcomeClient);
				link.transmit();
			}
		}
		else
		{
			assert(false);
		}
		
	}

	// TODO: MAKE PRIVATE
	size_t Server::addClient(Color &color, Link link)
	{

		size_t playerId = players.findFreeId();
		players.add(playerId, ServerPlayer(color, link)); // link will get copied here, problem?
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
				player.link.pushMessage(addObstacle);
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
		timeHandler.nextStep();
		float deltaTime = timeHandler.getDeltaTimef();

		// Read messages from clients
		ServerPlayers::Iterator playersIt;
		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		{			

			size_t playerId = playersIt->first;
			ServerPlayer player(playersIt->second);

			while (player.link.hasMessageOnQueue())
			{
				int messageType = player.link.popMessage();
				if (messageType == USER_CMD)
				{
					UserCmd *userCmd = player.link.getPoppedUserCmd();

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
					ShootCmd *shootCmd = player.link.getPoppedShootCmd();					
					size_t projectileId = worldModel.playerShoot(shootCmd->playerId);
					Projectile *projectile = (worldModel.getProjectiles())[projectileId];

					// send projectile to all clients
					AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(), projectile->getAngle(), projectile->getShooterId());
					pushMessageToAll(players, addProjectile);
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

			pushMessageToAll(players, updatePlayerObj);
		}

		// Send projectile updates
		WorldModel::ProjectileContainer::Iterator projectilesIt = worldModel.getProjectiles().begin();
		WorldModel::ProjectileContainer::Iterator projectilesEnd = worldModel.getProjectiles().end();
		for(; projectilesIt != projectilesEnd; ++projectilesIt)
		{			
			UpdateProjectile updateProjectile(projectilesIt->first, projectilesIt->second->getPos());
			
			pushMessageToAll(players, updateProjectile);
		}

		transmitAll(players);

	}
	
};
