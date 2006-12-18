#include "Client.h"
#include "Player.h"
//#include <iostream>
#include <string>
#include <algorithm>

namespace Prototype
{

	Client::Client() : messageSender(0), messageReciever(0), worldRenderer(WorldRenderer::HOLE_WORLD)
	{
		players.reserve(MAX_N_PLAYERS);
		//(worldModel.getPlayerObjs()).reserve(MAX_N_PLAYERS);
		//worldRenderer.setupProjection();
	}

	void Client::logic()
	{
		// Read messages from server
		while (messageReciever->getNMessages() > 0)
		{
			Message message = messageReciever->popMessage();
		
			std::cout << "message recieved, type: " << message.type << std::endl;

			// TODO - move this message handling code
			if (message.type == UPDATE_PLAYER)
			{
				UpdatePlayer *updatePlayer = (UpdatePlayer*)message.data;
				
				//// make a function of this and throw exception when id is't found instead!
				//WorldModel::PlayerObjContainer::iterator it =
				//	std::find_if(worldModel.getPlayerObjs().begin(),
				//	worldModel.getPlayerObjs().end(),
				//	HasPlayerId<PlayerObj>(updatePlayer->playerId));
				//if (it != worldModel.getPlayerObjs().end())
				//{
				//	PlayerObj *playerObj = *it;
				//	playerObj->pos = updatePlayer->pos;
				//	playerObj->angle = updatePlayer->angle;
				//}
				//else
				//{
				//	assert(false);
				//}

				WorldModel::PlayerObjContainer::iterator it;
				for (it = worldModel.getPlayerObjs().begin(); it != worldModel.getPlayerObjs().end(); it++)
				{
					PlayerObj *playerObj = *it;

					if (playerObj->getPlayerId() == updatePlayer->playerId)
					{
						playerObj->pos = updatePlayer->pos;
						playerObj->angle = updatePlayer->angle;
					}
				}
				
			}
		}


		// Send changes to server
		UserCmd *userCmd = new UserCmd();

		// If some key was pressed or released send message
		if (kh->changePressedToDownState() || kh->changeReleasedToUpState())
		{
			userCmd->cmd_left = kh->isDown(CMD_LEFT);
			userCmd->cmd_right = kh->isDown(CMD_RIGHT);
			userCmd->cmd_up = kh->isDown(CMD_UP);
			userCmd->cmd_down = kh->isDown(CMD_DOWN);
			userCmd->cmd_shoot = kh->isDown(CMD_SHOOT);

			Message message;
			message.type = USER_CMD;
			message.data = userCmd;

			sendMessage(message);
		}
	}

	void Client::render()
	{
		worldRenderer.setupProjection();
		worldRenderer.render(worldModel, players, 0);
	}

	void Client::sendMessage(Message message)
	{
		std::cout << "sending message, type: " << message.type << std::endl;
		
		message.time = timeHandler.getTime();
		
		messageSender->pushMessage(message);
		messageSender->transmit();
	}
	
	void Client::recieveMessages()
	{
	}

	void Client::addPlayer(const Color &playerColor, const Pos &playerPos)
	{
		int playerId = static_cast<int>(players.size());
		players.push_back(Player(playerColor));
		worldModel.getPlayerObjs().push_back(new PlayerObj(playerId, playerPos));
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		this->messageSender = messageSender;
		this->messageReciever = messageReciever;
	}

	void Client::setKeyHandler(KeyHandler *keyHandler)
	{
		this->kh = keyHandler;
	}

};
