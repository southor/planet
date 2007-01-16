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

	void Client::handleEvents()
	{
		PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

		kh.isPressed(CMD_ROTATE_LEFT);
		kh.isReleased(CMD_ROTATE_LEFT);
		if (kh.isDown(CMD_ROTATE_LEFT))
		{
			playerObj->angle += 0.1f;	
		}

		kh.isPressed(CMD_ROTATE_RIGHT);
		kh.isReleased(CMD_ROTATE_RIGHT);
		if (kh.isDown(CMD_ROTATE_RIGHT))
		{
			playerObj->angle -= 0.1f;	
		}
	}

	void Client::logic()
	{
		handleEvents();

		// Read messages from server
		while(link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == UPDATE_PLAYER_OBJ)
			{
				UpdatePlayerObj *updatePlayerObj = link.getPoppedUpdatePlayerObj();
				
				//PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
				PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
				playerObj->pos = updatePlayerObj->pos;
				
				if (playerId != updatePlayerObj->playerId)
				{
					playerObj->angle = updatePlayerObj->angle;
				}
			}
			else if (messageType == ADD_OBSTACLE)
			{
				AddObstacle *addObstacle = link.getPoppedAddObstacle();
				worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
			}
		}

		// If some key was pressed or released send message
		if (kh.changePressedToDownState() || kh.changeReleasedToUpState())
		{
			UserCmd userCmd;
			
			userCmd.cmdLeft = kh.isDown(CMD_LEFT);
			userCmd.cmdRight = kh.isDown(CMD_RIGHT);
			userCmd.cmdUp = kh.isDown(CMD_UP);
			userCmd.cmdDown = kh.isDown(CMD_DOWN);
			userCmd.cmdShoot = kh.isDown(CMD_SHOOT);
			userCmd.viewangle = ((worldModel.getPlayerObjs())[playerId])->angle;

			link.pushMessage(userCmd);
			link.transmit();
		}
	}

	void Client::render()
	{
		worldRenderer.setupProjection();
		worldRenderer.render(worldModel, players, 0);
	}

	void Client::initConnection()
	{
		// send init package to server
		InitClient initClient = InitClient(color);
		link.pushMessage(initClient);
		link.transmit();
	}

	void Client::initConnectionAgain()
	{
		// TODO: wait for this message to arive
		if (link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			if (messageType == WELCOME_CLIENT)
			{
				WelcomeClient *welcomeClient = link.getPoppedWelcomeClient();
			
				setPlayerId(welcomeClient->playerId);
			}
		}
		else
		{
			assert(false);
		}
	}

	void Client::addPlayer(const Color &playerColor, const Pos &playerPos)
	{
		size_t playerId = players.getSize();
		//size_t playerObjId = playerId; // for now
		players.add(playerId, Player(playerColor));
		//players[playerId].playerObjId = playerObjId;
		//worldModel.addPlayerObj(playerId, playerObjId, playerPos);
		worldModel.addPlayerObj(playerId, playerPos);
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		//this->messageSender = messageSender;
		//this->messageReciever = messageReciever;
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

	KeyHandler* Client::getKeyHandler()
	{
		return &kh;
	}

};
