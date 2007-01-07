#include "Server.h"

namespace Prototype
{

	size_t Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	{

		
		size_t playerId = players.add(ServerPlayer(color, messageSender, messageReciever));
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
	
	void Server::logic()
	{
		// Read messages from clients
		ServerPlayers::Iterator it;
		for (it = players.begin(); it != players.end(); ++it)
		{
			ServerPlayers::Pair pair = *it;
			size_t playerId = pair.first;
			ServerPlayer &player = pair.second;

			MessageSender *messageSender = player.link.messageSender;
			MessageReciever *messageReciever = player.link.messageReciever;
			


			while (messageReciever->getNMessages() > 0)
			{
				Message message = messageReciever->popMessage();

				if (message.type == USER_CMD)
				{
					UserCmd *userCmd = (UserCmd*)message.data;

					PlayerObj *playerObj = (worldModel.getPlayerObjs())[player.playerObjId];
					playerObj->movingForward = userCmd->cmd_up;
					playerObj->movingBackward = userCmd->cmd_down;
					playerObj->strafingLeft = userCmd->cmd_left;
					playerObj->strafingRight = userCmd->cmd_right;
					
					printf("userCmd.cmd_left = %d\n", userCmd->cmd_left);
				}			
				delete message.data;
			}

		}

		// update movements of players
		worldModel.updatePlayerObjMovements();


		// Send updates to clients
		for (it = players.begin(); it != players.end(); ++it)
		{
			ServerPlayers::Pair pair = *it;
			ServerPlayer &player = pair.second;

			MessageSender *messageSender = player.link.messageSender;
			MessageReciever *messageReciever = player.link.messageReciever;


			

			WorldModel::PlayerObjContainer::Iterator it;
			for(it = worldModel.getPlayerObjs().begin(); it != worldModel.getPlayerObjs().end(); ++it)
			{
				size_t playerObjId = it->first;
				PlayerObj *playerObj = it->second;
				Message message(UPDATE_PLAYER, new UpdatePlayerObj(playerObjId, playerObj->pos, playerObj->angle));
				messageSender->pushMessage(message);
			}

			
			
			messageSender->transmit();
		}
	}
	
};
