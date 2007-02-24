#include "Client.h"
#include "Player.h"
//#include <iostream>
#include <string>
#include <algorithm>

namespace Prototype
{

	Client::Client() : worldRenderer(WorldRenderer::FOLLOW_PLAYER), connectionPhase(0),
		mousePos(0.0f, 0.0f), aimMode(KEYBOARD)
	{
	}

	void Client::handleEvents()
	{
		if (aimMode == KEYBOARD)
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
	}

	void Client::logic()
	{
		timeHandler.nextStep();
		
		assert(worldModel.isConsistent());
		if (timeHandler.isNewTick())
		{
			link.retrieve(timeHandler.getTime());
			
			//printf("CLIENT getNMessagesOnQueue: %d\n", link.getNMessagesOnQueue());
			

			// Read messages from server
			while(link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				switch(messageType)
				{
				case UPDATE_PLAYER_OBJ:
					{
						UpdatePlayerObj *updatePlayerObj = link.getPoppedData<UpdatePlayerObj>();
						
						//PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
						PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
						playerObj->pos = updatePlayerObj->pos;
						//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
						if (playerId == updatePlayerObj->playerId)
						{

						}
						else
						{
							playerObj->angle = updatePlayerObj->angle;
						}
					}
					break;
				case ADD_PLAYER_OBJ:
					{
						AddPlayerObj *addPlayerObj = link.getPoppedData<AddPlayerObj>();
						addPlayer(addPlayerObj->playerId, addPlayerObj->color, addPlayerObj->pos);				

						if (connectionPhase == 2) connectionPhase++; 
					}
					break;
				case ADD_OBSTACLE:
					{
						AddObstacle *addObstacle = link.getPoppedData<AddObstacle>();
						worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
						
						if (connectionPhase == 3) connectionPhase++; 
					}
					break;
				case ADD_PROJECTILE:
					{
						printf("CLIENT: handling add_projectile @ %d\n", timeHandler.getTime());

						AddProjectile *addProjectile = link.getPoppedData<AddProjectile>();
						worldModel.addProjectile(addProjectile->projectileId, static_cast<Projectile::Type>(addProjectile->type), addProjectile->pos, addProjectile->angle, addProjectile->shooterId);
					}
					break;
				case UPDATE_PROJECTILE:
					{
						UpdateProjectile *updateProjectile = link.getPoppedData<UpdateProjectile>();
						Projectile *projectile = (worldModel.getProjectiles())[updateProjectile->projectileId];
						projectile->setPos(updateProjectile->pos);
					}
					break;
				case REMOVE_PROJECTILE:
					{
						RemoveProjectile *removeProjectile = link.getPoppedData<RemoveProjectile>();
						worldRenderer.projectileHit((worldModel.getProjectiles())[removeProjectile->projectileId], removeProjectile->hitPosition);
						worldModel.getProjectiles().remove(removeProjectile->projectileId);					
					}
					break;
				case KILL:
					{
						Kill *kill = link.getPoppedData<Kill>();
						PlayerObj *killer = (worldModel.getPlayerObjs())[kill->killerId];
						PlayerObj *killed = (worldModel.getPlayerObjs())[kill->killedId];
						killed->pos = kill->respawnPos;
						killed->setAmmoSupply(static_cast<int>(killed->pos.x + killed->pos.y + killer->pos.x + killer->pos.y));
						if (killed->getAmmoCurrentWeapon() <= 0) killed->switchWeapon();
					}
					break;
				case START_GAME:
					timeHandler.reset();
					break;
				default:
					break;
				};


			}

			if (connectionPhase == 4)
			{			


				handleEvents();
				int time = timeHandler.getStepTime();

				

				//if (kh.isPressed(CMD_SHOOT))
				//{
				//	ShootCmd shootCmd = {playerId};
				//	
				//	link.pushMessage(shootCmd);
				//	link.transmit();
				//}

				PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

				if (kh.isPressed(CMD_SWITCH_WEAPON))
				{
					playerObj->switchWeapon();
				}
				
				bool wasKeyEvent = kh.changePressedToDownState() || kh.changeReleasedToUpState();

				// handle shooting
				if (kh.isDown(CMD_SHOOT))
				{					
					if (playerObj->canShoot(time))
					{
						Projectile::Type weapon = playerObj->getCurrentWeapon();
						playerObj->shoot(time);
						ShootCmd shootCmd(playerId, weapon);
						link.pushMessage(shootCmd, timeHandler.getTime(), timeHandler.getStepTick());
					}
				}

				// If some key was pressed or released send message
				//if (wasKeyEvent || (this->mousePosChanged && (this->aimMode == MOUSE)))
				if (!kh.isDown(CMD_SWITCH_WEAPON))
				{
					if (this->mousePosChanged && (this->aimMode == MOUSE))
					{
						updatePlayerObjAngle();
						this->mousePosChanged = false;
					}

					int stateCmds = getUserInput()->getCurrentStates().getStates();
					float aimangle = ((worldModel.getPlayerObjs())[playerId])->angle.getFloat();
					
					UserCmd userCmd(stateCmds, aimangle);
					//printf("CLIENT: sending usercmd with tick: %f @ time %d\n", timeHandler.getStepTick(), timeHandler.getStepTime());
					link.pushMessage(userCmd, timeHandler.getTime(), timeHandler.getStepTick());
					
				}

				// transmit any messages
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
			link.pushMessage(initClient, timeHandler.getTime(), timeHandler.getStepTick());
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == 1)
		{
			link.retrieve(timeHandler.getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == WELCOME_CLIENT)
				{
					WelcomeClient *welcomeClient = link.getPoppedData<WelcomeClient>();
				
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
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerId]);
		}



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

	void Client::addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos)
	{
		
		//size_t playerId = players.getSize();
		//size_t playerObjId = playerId; // for now
		players.add(playerId, Player(playerColor));
		//players[playerId].playerObjId = playerObjId;
		//worldModel.addPlayerObj(playerId, playerObjId, playerPos);
		
		// if this is me
		bool isMe = (playerId == this->playerId);

		worldModel.addPlayerObj(playerId, playerPos, isMe);
		
		
		if (isMe)
		{
			
			// set ammo supply
			(worldModel.getPlayerObjs())[playerId]->setAmmoSupply(static_cast<int>(playerPos.x + playerPos.y));
		}
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

	void Client::setCurrentMousePos(Vec2<int> mouseScreenPos)
	{
		mousePosChanged = true;
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
			
			viewportHandler.renderArea.size = WorldRenderer::RENDER_SIZE; // set current render area
			viewportHandler.renderArea.setCenterPos(playerObj->pos); // set current render area
			mousePos = viewportHandler.screenToGame(mouseScreenPos); // mouse position in game
		}
	}

	void Client::updatePlayerObjAngle()
	{
		//std::cout << mouseScreenPos.y << std::endl;
		
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
			
			//viewportHandler.renderArea.size = WorldRenderer::RENDER_SIZE; // set current render area
			//viewportHandler.renderArea.setCenterPos(playerObj->pos); // set current render area
			//Pos mousePos = viewportHandler.screenToGame(mouseScreenPos); // mouse position in game
			//std::cout << mousePos.y << std::endl;

			float angle;
			Vec aimVec = mousePos - playerObj->pos;
			aimVec.normalize();
			if ((aimVec.x > 0.5) || (aimVec.x < 0.5f))
			{
				angle = asin(aimVec.y);
				if (aimVec.x < 0.0f) angle = PI_F - angle;
			}
			else
			{
				angle = acos(aimVec.x);
				if (aimVec.y < 0.0f) angle = -angle;
			}
			playerObj->angle = angle;

		}
	}

};
