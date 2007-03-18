#include "Client.h"
#include "Game.h"
#include <string>
#include <algorithm>

namespace Planet
{
	Client::Client() : ClientGlobalAccess(&clientGlobalObj), connectionPhase(0), planet(&clientGlobalObj)
	{}

	void Client::init()
	{
		planet.init(currentMap);
		ship.setPlanetBody(planet.getPlanetBody());
		sight.setCamera(&camera);
		sight.setPlanetBody(planet.getPlanetBody());
		
		getTimeHandler()->reset();
	}

	Client::~Client()
	{
		DeleteSecond<ClientPlayers::Pair> deleteSecond;
		ForEach(players.begin(), players.end(), deleteSecond);
	}

	void Client::handleServerMessages()
	{
		link.retrieve(getTimeHandler()->getTime());
		
		while(link.hasMessageOnQueue())
		{
			int messageType = link.popMessage();
			switch(messageType)
			{
			case UPDATE_PLAYER_OBJ:
				{
					UpdatePlayerObj *updatePlayerObj = link.getPoppedData<UpdatePlayerObj>();
					
					PlayerObj *playerObj = (planet.getPlayerObjs())[updatePlayerObj->playerId];
					//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
					
					if (playerId == updatePlayerObj->playerId)
					{
						bool differ = playerObj->setTickDataAndCompare(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
						if (differ)
						{
							//std::cout << "old prediction differ!" << std::endl;
							predictionHandler.serverInput(playerId, link.getPoppedTick());						
						}
						else
						{
							//std::cout << "old prediction ok!" << std::endl;
						}
					}
					else
					{
						playerObj->setTickData(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
					}
				}
				break;
			case ADD_PLAYER_OBJ:
				{
					AddPlayerObj *addPlayerObj = link.getPoppedData<AddPlayerObj>();
					addPlayer(addPlayerObj->playerId, addPlayerObj->color, addPlayerObj->pos, link.getPoppedTick());				

					if (connectionPhase == ClientPhase::GET_ADDPLAYEROBJ)
						connectionPhase++;
				}
				break;
			case ADD_OBSTACLE:
				{
					AddObstacle *addObstacle = link.getPoppedData<AddObstacle>();
					worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
					
					if (connectionPhase == ClientPhase::GET_ADDOBSTACLE)
						connectionPhase++; 
				}
				break;
			case ADD_PROJECTILE:
				{
					//printf("CLIENT: handling add_projectile @ %d\n", timeHandler.getTime());

					AddProjectile *addProjectile = link.getPoppedData<AddProjectile>();					
					bool projectileAlreadyCreated = false;
					if (static_cast<PlayerId>(addProjectile->shooterId) == playerId)
					{
						projectileAlreadyCreated = worldModel.getProjectiles().exists(addProjectile->projectileId);
					}
					if (!projectileAlreadyCreated)
					{
						worldModel.addProjectile(addProjectile->projectileId, static_cast<Projectile::Type>(addProjectile->type), addProjectile->pos, addProjectile->angle, addProjectile->shooterId, link.getPoppedTick(), addProjectile->objLag);
					}
				}
				break;
			case UPDATE_PROJECTILE:
				{
					//printf("CLIENT: handling update_projectile @ %d\n", timeHandler.getTime());
					
					UpdateProjectile *updateProjectile = link.getPoppedData<UpdateProjectile>();
					Projectile *projectile = (worldModel.getProjectiles())[updateProjectile->projectileId];
					projectile->setTickData(link.getPoppedTick(), updateProjectile->pos);
					//std::cout << "client projectile:  pos.x = " << projectile->getPos().x << "  pos.y = " << projectile->getPos().y << std::endl;
				}
				break;
			case REMOVE_PROJECTILE:
				{
					//printf("CLIENT: handling remove_projectile @ %d\n", timeHandler.getTime());
					
					RemoveProjectile *removeProjectile = link.getPoppedData<RemoveProjectile>();
					//worldRenderer.projectileHit((worldModel.getProjectiles())[removeProjectile->projectileId], removeProjectile->hitPosition);
					worldModel.getProjectiles().remove(removeProjectile->projectileId);
				}
				break;
			case PROJECTILE_HIT:
				{
					//printf("CLIENT: handling remove_projectile @ %d\n", timeHandler.getTime());
					
					ProjectileHit *projectileHit = link.getPoppedData<ProjectileHit>();
					worldRenderer.projectileHit((worldModel.getProjectiles())[projectileHit->projectileId], projectileHit->hitPosition);
					worldModel.getProjectiles().remove(projectileHit->projectileId);
				}
				break;
			case START_GAME:
				break;
			case SET_TICK_0_TIME:
				getTimeHandler()->enterTick0Time(*(link.getPoppedData<SetTick0Time>()));
				//std::cout << "tick0Time: " << timeHandler.getTick0Time() << std::endl;
				break;
			default:
				break;
			};
		}
	}

	void Client::runStep()
	{
		assert(isConsistent());
		
		getTimeHandler()->nextStep();
		if (connectionPhase == ClientPhase::RUNNING)
		{
			if (getTimeHandler()->isNewTick())
			{
				assert(predictionHandler.isConsistent());
				
				int currentTick = static_cast<int>(getStepTick());
				

				// calculate current objectLag				
				double tmp = static_cast<double>(link.getCurrentLag() - getTimeHandler()->getTick0Time());
				this->currentObjLag = static_cast<int>(tmp * OBJECT_LAG_MODIFIER + OBJECT_LAG_ADD_TIME)
													/ TimeHandler::TICK_DELTA_TIME + OBJECT_LAG_ADD_TICK;

				// get userCmd
				UserCmd userCmd;
				getCurrentUserCmd(userCmd);
				userCmd.objLag = this->currentObjLag; // send currentObjLag to server
				
				// store userCmd
				predictionHandler.setUserCmd(userCmd, currentTick);
				
				// push userCmd to serverlink
				link.pushMessage(userCmd, getTimeHandler()->getTime(), currentTick);

				// transmit messages
				link.transmit();

				//perform shooting
				PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
				playerObj->updateToTickData(currentTick);
				playerObj->setUserCmd(&userCmd);
				worldModel.handlePlayerShooting(playerId);

				// perform prediction
				predictionHandler.predict(playerId, currentTick + 1);
			}
			else
			{
				requestRender = true;
			}
		}
		else
		{
			handleServerMessages();
		}

		//ship.moveUp = userInputHandler.getCurrentState(Cmds::TMP_UP);
		//ship.moveDown = userInputHandler.getCurrentState(Cmds::TMP_DOWN);
		//ship.moveLeft = userInputHandler.getCurrentState(Cmds::TMP_LEFT);
		//ship.moveRight = userInputHandler.getCurrentState(Cmds::TMP_RIGHT);
		//	
		//// Logic
		//ship.logic(sight.position);

		//camera.update(ship.position, ship.reference);

		//sight.update(userInputHandler.getMouseScreenPos(), Game::WINDOW_SIZE.x, Game::WINDOW_SIZE.y);
	}

	bool Client::initConnection()
	{
		if (connectionPhase == ClientPhase::SEND_INITCLIENT)
		{
			// send init package to server
			InitClient initClient = InitClient(color);
			link.pushMessage(initClient, getTimeHandler()->getTime(), static_cast<int>(getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::WAIT_WELCOME_CLIENT)
		{
			link.retrieve(getTimeHandler()->getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == WELCOME_CLIENT)
				{
					WelcomeClient *welcomeClient = link.getPoppedData<WelcomeClient>();
				
					setPlayerId(welcomeClient->playerId);
					currentMap = "maps/test/";

					connectionPhase++;
				}
				else
				{
					assert(false);
				}
			}
		}

		if (connectionPhase == ClientPhase::SYNC_SEND_PING)
		{
			// send ping to server with current client time
			SyncPing syncPing(playerId, getTimeHandler()->getTime());
			link.pushMessage(syncPing, getTimeHandler()->getTime(), static_cast<int>(getStepTick()));
			link.transmit();

			connectionPhase++;
		}

		if (connectionPhase == ClientPhase::SYNC_GET_PONG)
		{
			// get pong pack from server with server time and time when ping was sent
			link.retrieve(getTimeHandler()->getTime());
			if (link.hasMessageOnQueue())
			{
				int messageType = link.popMessage();
				if (messageType == SYNC_PONG)
				{
					SyncPong *syncPong = link.getPoppedData<SyncPong>();
					
					int clientTime = getTimeHandler()->getTime();
					int serverTime = syncPong->time;

					int pingTime = clientTime - syncPong->pingSendTime;
					int serverClientDiff = (serverTime + pingTime/2) - clientTime;

					printf("Adjusting client time with diff: %d\n", serverClientDiff);

					// Modify client time to match server time
					getTimeHandler()->incrementTime(serverClientDiff);
 
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

	void Client::renderAndUpdate()
	{
		//render
		//planet.getPlanetBody()->render();
		planetRenderer.render(planet);

		// Disable lights for ship and sight rendering
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		ship.render();
		sight.render();

		planetRenderer.render(ship.position, ship.direction);

		requestRender = false;


	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

};
