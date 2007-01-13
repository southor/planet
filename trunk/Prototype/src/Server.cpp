#include "Server.h"

namespace Prototype
{

	Server::Server()
	{
		Rectangle obstacleArea(50.0f, 70.0f, 150.0f, 80.0f);
		worldModel.addObstacle(obstacleArea);
		obstacleArea.pos.x = 300.0f;
		worldModel.addObstacle(obstacleArea);
	}

	size_t Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	{

		size_t playerId = players.findFreeId();
		players.add(playerId, ServerPlayer(color, messageSender, messageReciever));
		return playerId;
		

		
		//ServerClient client(messageSender, messageReciever);

		//addClient(client);
	}

	//size_t Server::addPlayer(const ServerPlayer &player)
	//{
	//	return players.add(player);
	//	//clients.push_back(client);
	//}

	size_t Server::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		size_t playerObjId = worldModel.addPlayerObj(playerId, playerPos);
		players[playerId].playerObjId = playerObjId;
		return playerObjId;
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

					PlayerObj *playerObj = (worldModel.getPlayerObjs())[player.playerObjId];
					playerObj->movingForward = userCmd->cmdUp;
					playerObj->movingBackward = userCmd->cmdDown;
					playerObj->strafingLeft = userCmd->cmdLeft;
					playerObj->strafingRight = userCmd->cmdRight;
				}
			}


			//MessageSender *messageSender = player.link.messageSender;
			//MessageReciever *messageReciever = player.link.messageReciever;

			//while (messageReciever->getNMessages() > 0)
			//{
			//	Message message = messageReciever->popMessage();

			//	if (message.type == USER_CMD)
			//	{
			//		UserCmd *userCmd = reinterpret_cast<UserCmd*>(message.data);

			//		PlayerObj *playerObj = (worldModel.getPlayerObjs())[player.playerObjId];
			//		playerObj->movingForward = userCmd->cmdUp;
			//		playerObj->movingBackward = userCmd->cmdDown;
			//		playerObj->strafingLeft = userCmd->cmdLeft;
			//		playerObj->strafingRight = userCmd->cmdRight;
			//		
			//		printf("userCmd.cmdLeft = %d\n", userCmd->cmdLeft);
			//	}			
			//	delete message.data;
			//}

		}

		// update movements of players
		worldModel.updatePlayerObjMovements();


		// Send updates to clients
		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
		{
			ServerPlayer player(playersIt->second);

			WorldModel::PlayerObjContainer::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
			WorldModel::PlayerObjContainer::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
			for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
			{
				size_t playerObjId = playerObjsIt->first;
				PlayerObj *playerObj = playerObjsIt->second;
				UpdatePlayerObj updatePlayerObj(playerObjId, playerObj->pos, playerObj->angle);
				player.link.pushMessage(updatePlayerObj);
			}

			player.link.transmit();

			//MessageSender *messageSender = player.link.messageSender;
			//MessageReciever *messageReciever = player.link.messageReciever;

			//WorldModel::PlayerObjContainer::Iterator it;
			//for(it = worldModel.getPlayerObjs().begin(); it != worldModel.getPlayerObjs().end(); ++it)
			//{
			//	size_t playerObjId = it->first;
			//	PlayerObj *playerObj = it->second;
			//	Message message(UPDATE_PLAYER, new UpdatePlayerObj(playerObjId, playerObj->pos, playerObj->angle));
			//	messageSender->pushMessage(message);
			//}

			//messageSender->transmit();
		}
	}
	
};
