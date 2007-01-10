#include "Client.h"
#include "Player.h"
//#include <iostream>
#include <string>
#include <algorithm>

namespace Prototype
{

	Client::Client() : worldRenderer(WorldRenderer::HOLE_WORLD)
	{
	}

	//Client::Client() : messageSender(0), messageReciever(0), worldRenderer(WorldRenderer::HOLE_WORLD)
	//{
	//}

	void Client::logic()
	{
		// Read messages from server
		while(link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == UPDATE_PLAYER_OBJ)
			{
				UpdatePlayerObj *updatePlayerObj = link.getPoppedUpdatePlayerObj();

				PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
				playerObj->pos = updatePlayerObj->pos;
				playerObj->angle = updatePlayerObj->angle;	
			}
		}

		//while (messageReciever->getNMessages() > 0)
		//{
		//	Message message = messageReciever->popMessage();
		//
		//	std::cout << "message recieved, type: " << message.type << std::endl;

		//	// TODO - move this message handling code
		//	if (message.type == UPDATE_PLAYER)
		//	{
		//		UpdatePlayerObj *updatePlayerObj = (UpdatePlayerObj*)message.data;

		//		PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
		//		playerObj->pos = updatePlayerObj->pos;
		//		playerObj->angle = updatePlayerObj->angle;	
		//	}
		//}

		

		// If some key was pressed or released send message
		if (kh->changePressedToDownState() || kh->changeReleasedToUpState())
		{
			UserCmd userCmd;
			
			userCmd.cmdLeft = kh->isDown(CMD_LEFT);
			userCmd.cmdRight = kh->isDown(CMD_RIGHT);
			userCmd.cmdUp = kh->isDown(CMD_UP);
			userCmd.cmdDown = kh->isDown(CMD_DOWN);
			userCmd.cmdShoot = kh->isDown(CMD_SHOOT);

			link.pushMessage(userCmd);
			link.transmit();
		}

		

		//// Send changes to server
		//UserCmd *userCmd = new UserCmd();

		//// If some key was pressed or released send message
		//if (kh->changePressedToDownState() || kh->changeReleasedToUpState())
		//{
		//	userCmd->cmdLeft = kh->isDown(CMD_LEFT);
		//	userCmd->cmdRight = kh->isDown(CMD_RIGHT);
		//	userCmd->cmdUp = kh->isDown(CMD_UP);
		//	userCmd->cmdDown = kh->isDown(CMD_DOWN);
		//	userCmd->cmdShoot = kh->isDown(CMD_SHOOT);

		//	Message message;
		//	message.type = USER_CMD;
		//	message.data = userCmd;

		//	sendMessage(message);
		//}
	}

	void Client::render()
	{
		worldRenderer.setupProjection();
		worldRenderer.render(worldModel, players, 0);
	}

	//void Client::sendMessage(Message message)
	//{
	//	std::cout << "sending message, type: " << message.type << std::endl;
	//	
	//	message.time = timeHandler.getTime();
	//	
	//	messageSender->pushMessage(message);
	//	messageSender->transmit();
	//}
	
	void Client::recieveMessages()
	{
	}

	void Client::addPlayer(const Color &playerColor, const Pos &playerPos)
	{
		size_t playerId = players.getSize();
		size_t playerObjId = playerId; // for now
		players.add(playerId, Player(playerColor));
		players[playerId].playerObjId = playerObjId;
		worldModel.addPlayerObj(playerId, playerObjId, playerPos);
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		//this->messageSender = messageSender;
		//this->messageReciever = messageReciever;
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

	void Client::setKeyHandler(KeyHandler *keyHandler)
	{
		this->kh = keyHandler;
	}

};
