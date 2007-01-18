#include "Client.h"
#include "Player.h"
//#include <iostream>
#include <string>
#include <algorithm>

namespace Prototype
{

	Client::Client() : worldRenderer(WorldRenderer::HOLE_WORLD), connectionPhase(0)
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
			else if (messageType == ADD_PLAYER_OBJ)
			{
				AddPlayerObj *addPlayerObj = link.getPoppedAddPlayerObj();
				addPlayer(addPlayerObj->color, addPlayerObj->pos);

				if (connectionPhase == 2) connectionPhase++; 
			}
			else if (messageType == ADD_OBSTACLE)
			{
				AddObstacle *addObstacle = link.getPoppedAddObstacle();
				worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
				
				if (connectionPhase == 3) connectionPhase++; 
			}
			else if (messageType == ADD_PROJECTILE)
			{
				AddProjectile *addProjectile = link.getPoppedAddProjectile();
				worldModel.addProjectile(addProjectile->projectileId, static_cast<Projectile::Type>(addProjectile->type), addProjectile->pos, addProjectile->angle, addProjectile->shooterId);
			}
			else if (messageType == UPDATE_PROJECTILE)
			{
				UpdateProjectile *updateProjectile = link.getPoppedUpdateProjectile();
				Projectile *projectile = (worldModel.getProjectiles())[updateProjectile->projectileId];
				projectile->pos = updateProjectile->pos;
			}
			else if (messageType == REMOVE_PROJECTILE)
			{
				worldModel.getProjectiles().remove(link.getPoppedRemoveProjectile()->projectileId);
			}
		}

		if (connectionPhase == 4)
		{
			handleEvents();

			if (kh.isPressed(CMD_SHOOT))
			{
				ShootCmd shootCmd = {playerId};
				
				link.pushMessage(shootCmd);
				link.transmit();
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
	}

	bool Client::initConnection()
	{
		if (connectionPhase == 0)
		{
			// send init package to server
			InitClient initClient = InitClient(color);
			link.pushMessage(initClient);
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == 1)
		{
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == WELCOME_CLIENT)
				{
					WelcomeClient *welcomeClient = link.getPoppedWelcomeClient();
				
					setPlayerId(welcomeClient->playerId);
					
					connectionPhase++;
					
					return true;
				}
				else
				{
					assert(false);
				}
			}
		}
		
		return false;
	}

	void Client::render()
	{
		worldRenderer.setupProjection();
		worldRenderer.render(worldModel, players, 0);



		//// testing Line testing cross point
		
		//static float x = 0.0f;
		//x += 0.5f;
		//Line testLine1(0.0f, 0.0f, 200.0f, 200.0f);
		//Line testLine2(0.0f, x, 200.0f, 0.0f);
		//Rectangle crossBox(testLine1.getPosAlong(testLine1.crossPoint(testLine2)), 15.0f);

		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		//WorldRenderer::renderLine(testLine1, 2.0f, 1.0f);
		//glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		//WorldRenderer::renderLine(testLine2, 2.0f, 1.0f);
		//glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
		//WorldRenderer::renderRectangle(crossBox, GL_QUADS);

		//Rectangle rect;
		//(worldModel.getPlayerObjs())[0]->getRectangle(rect);		
		//Line leftTest(rect.getLeftLine());
		//Line rightTest(rect.getRightLine());
		//Line topTest(rect.getTopLine());
		//Line bottomTest(rect.getBottomLine());

		//glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
		//WorldRenderer::renderLine(leftTest, 1.0f, 1.0f);
		//WorldRenderer::renderLine(rightTest, 1.0f, 1.0f);
		//WorldRenderer::renderLine(topTest, 1.0f, 1.0f);
		//WorldRenderer::renderLine(bottomTest, 1.0f, 1.0f);
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
