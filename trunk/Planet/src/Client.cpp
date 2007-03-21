#include "Client.h"
#include "Game.h"
#include <string>
#include <algorithm>

namespace Planet
{
	const double Client::OBJECT_LAG_MODIFIER = 1.2;
	const int Client::OBJECT_LAG_ADD_TIME = 18;
	const int Client::OBJECT_LAG_ADD_TICK = 1;

	Client::Client() : ClientGlobalAccess(&clientGlobalObj), connectionPhase(0),
						planet(&clientGlobalObj), requestRender(false), currentObjLag(0)
	{
		predictionHandler.setPlanet(&planet);
		assert(predictionHandler.isConsistent());
	}

	void Client::init()
	{
		planet.init(currentMap);
		sight.setCamera(&camera);
		sight.setPlanetBody(planet.getPlanetBody());
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
					assert(updatePlayerObj->isConsistent());
					
					PlayerObj *playerObj = (planet.getPlayerObjs())[updatePlayerObj->playerId];
					//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
					
					
					if (playerId == updatePlayerObj->playerId)
					{						
						bool differ = playerObj->setTickDataAndCompare(link.getPoppedTick(), updatePlayerObj);
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
						playerObj->setTickData(link.getPoppedTick(), updatePlayerObj);
					}
				}
				break;
			case ADD_PLAYER_OBJ:
				{
					AddPlayerObj *addPlayerObj = link.getPoppedData<AddPlayerObj>();
					assert(addPlayerObj->isConsistent());
					addPlayer(addPlayerObj->playerId, addPlayerObj->color, addPlayerObj->pos, addPlayerObj->aimPos, link.getPoppedTick());

					printf("adding client object: %d\n", addPlayerObj->playerId);

					if (connectionPhase == ClientPhase::GET_ADDPLAYEROBJ)
						connectionPhase++;
				}
				break;
			/*
			case ADD_OBSTACLE:
				{
					AddObstacle *addObstacle = link.getPoppedData<AddObstacle>();
					worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
					
					if (connectionPhase == ClientPhase::GET_ADDOBSTACLE)
						connectionPhase++; 
				}
				break;
			*/
			case ADD_PROJECTILE:
				{
					//printf("CLIENT: handling add_projectile @ %d\n", timeHandler.getTime());

					AddProjectile *addProjectile = link.getPoppedData<AddProjectile>();
					assert(addProjectile->isConsistent());

					bool projectileAlreadyCreated = false;
					if (static_cast<PlayerId>(addProjectile->shooterId) == playerId)
					{
						projectileAlreadyCreated = planet.getProjectiles().exists(addProjectile->projectileId);
					}
					if (!projectileAlreadyCreated)
					{
						planet.addProjectile(addProjectile->projectileId, addProjectile);
					}
				}
				break;
			case UPDATE_PROJECTILE:
				{
					//printf("CLIENT: handling update_projectile @ %d\n", timeHandler.getTime());
					
					UpdateProjectile *updateProjectile = link.getPoppedData<UpdateProjectile>();
					assert(updateProjectile->isConsistent());

					Projectile *projectile = (planet.getProjectiles())[updateProjectile->projectileId];
					projectile->setTickData(link.getPoppedTick(), updateProjectile->pos);
					//std::cout << "client projectile:  pos.x = " << projectile->getPos().x << "  pos.y = " << projectile->getPos().y << std::endl;
				}
				break;
			case REMOVE_PROJECTILE:
				{
					//printf("CLIENT: handling remove_projectile @ %d\n", timeHandler.getTime());
					
					RemoveProjectile *removeProjectile = link.getPoppedData<RemoveProjectile>();
					assert(removeProjectile->isConsistent());
					//worldRenderer.projectileHit((worldModel.getProjectiles())[removeProjectile->projectileId], removeProjectile->hitPosition);
					planet.getProjectiles().remove(removeProjectile->projectileId);
				}
				break;
			case PROJECTILE_HIT:
				{
					//printf("CLIENT: handling remove_projectile @ %d\n", timeHandler.getTime());
					
					ProjectileHit *projectileHit = link.getPoppedData<ProjectileHit>();
					assert(projectileHit->isConsistent());
					//planetRenderer.projectileHit((planet.getProjectiles())[projectileHit->projectileId], projectileHit->hitPosition);
					planet.getProjectiles().remove(projectileHit->projectileId);
				}
				break;
			case START_GAME:
				break;
			case SET_TICK_0_TIME:
				getTimeHandler()->enterTick0Time((link.getPoppedData<SetTick0Time>())->tick0Time);
				//std::cout << "tick0Time: " << timeHandler.getTick0Time() << std::endl;
				break;
			default:
				break;
			};
		}
	}

	void Client::getCurrentUserCmd(UserCmd &userCmd)
	{
		//static int lastTick = -100;
		//assert(timeHandler.getStepTick() >= (lastTick +1));
		//lastTick = timeHandler.getStepTick();

		// get current tick
		int currentTick = static_cast<int>(getStepTick());
		
		// get playerobj
		PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];
		playerObj->updateToTickData(currentTick);
		if (!playerObj)
		{
			userCmd.clear();
			return;
		}

		// get previous user command
		UserCmd preUserCmd;
		predictionHandler.getUserCmd(preUserCmd, currentTick - 1);

		// get userCmd to start with (could contain postponed actionCmds from previous tick)
		predictionHandler.getUserCmd(userCmd, currentTick);		

		
		// get stateCmds
		StateCmds stateCmds = userInputHandler.getCurrentStates();

		// set stateCmds, aimPos and firstProjectileId
		userCmd.stateCmds = stateCmds;
		userCmd.aimPos = sight.position;
		userCmd.firstProjectileId = getIdGenerator()->getNextId();

		// set shooting action and nShots
		assert(userCmd.nShots == 0);
		userCmd.firstShotTick = playerObj->getNextShootTick();
		//if (userCmd.shootAction == UserCmd::CONTINUE_SHOOTING)		
		if (userCmd.isShooting())
		{			
			assert(userCmd.firstShotTick >= static_cast<Tickf>(currentTick));
			userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick);
		}
		//else
		//{
		//	assert(userCmd.shootAction == UserCmd::NOT_SHOOTING);
		//}
		bool startShootingThisTick = false;
		while(userInputHandler.hasActionCmdOnQueue())
		{
			int actionCmd = userInputHandler.popActionCmd();

			if (actionCmd == Cmds::START_SHOOTING)
			{
				if (DEBUG_SHOOTING) std::cout << "    -- start shooting" << std::endl;
				
				userCmd.shooting = true;
				startShootingThisTick = true;
				userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick);				
			}
			else if (actionCmd == Cmds::STOP_SHOOTING)
			{
				if (DEBUG_SHOOTING) std::cout << "    -- stop shooting" << std::endl;
				
				userCmd.shooting = false;
				if (startShootingThisTick && (userCmd.nShots >= 1)) userCmd.nShots = 1;
				else userCmd.nShots = 0;				
			}
			else if (actionCmd == Cmds::SWITCH_WEAPON)
			{
				if (DEBUG_SHOOTING) std::cout << "    -- switch weapon" << std::endl;

				Projectile::Type nextWeapon = playerObj->getNextWeapon(userCmd.weapon);
				if (userCmd.nShots > 0)
				{
					UserCmd nextUserCmd;
					userCmd.assumeNext(nextUserCmd);
					nextUserCmd.weapon = nextWeapon;
					predictionHandler.setUserCmd(nextUserCmd, currentTick + 1);
					break;
				}
				else
				{
					userCmd.weapon = nextWeapon;
				}
			}
		}
		if ((userCmd.isShooting() || (userCmd.nShots > 0)) && DEBUG_SHOOTING)
		{			
			//userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick, userCmd.shootAction == UserCmd::CONTINUE_SHOOTING);
			std::cout << "-- " << currentTick << " shooting, nShots = " << userCmd.nShots;
			std::cout << "   playerObj.nextShootTick  = " << playerObj->getNextShootTick() << std::endl;
		}

		
		assert(userCmd.isConsistent(currentTick));
	}

	void Client::runStep()
	{
		//printf("Client::runStep(), phase: %d\n", connectionPhase);
	
		assert(isConsistent());

		link.retrieve(getTimeHandler()->getTime());
		
		getTimeHandler()->nextStep();
		if (connectionPhase == ClientPhase::RUNNING)
		{
			if (getTimeHandler()->isNewTick())
			{
				assert(predictionHandler.isConsistent());
				
				int currentTick = static_cast<int>(getStepTick());
				

				// calculate current objectLag
				int tick0Time = getTimeHandler()->getTick0Time();
				//std::cout << "tick0Time: " << tick0Time << std::endl;
				double tmp = static_cast<double>(link.getCurrentLag() - tick0Time);
				this->currentObjLag = static_cast<int>(tmp * OBJECT_LAG_MODIFIER + OBJECT_LAG_ADD_TIME)
													/ TimeHandler::TICK_DELTA_TIME + OBJECT_LAG_ADD_TICK;
				assert(this->currentObjLag >= 0);

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
				PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];
				playerObj->updateToTickData(currentTick);
				playerObj->setUserCmd(&userCmd);
//				planet.handlePlayerShooting(playerId);

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


		PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];

		Ship *ship = playerObj->getShip();
		camera.update(ship->position, ship->reference);

		sight.update(userInputHandler.getMouseScreenPos(), Game::WINDOW_SIZE.x, Game::WINDOW_SIZE.y);
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
					getIdGenerator()->setPlayerId(welcomeClient->playerId);
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


		PlayerObj *playerObj = (planet.getPlayerObjs())[playerId];

		playerObj->getShip()->render();
		sight.render();

		//planetRenderer.render(ship.position, ship.getDirection());

		requestRender = false;


	}

	void Client::addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, const Pos &playerAimPos, int tick)
	{
		
		//size_t playerId = players.getSize();
		//size_t playerObjId = playerId; // for now
		
		players.add(playerId, new Player(playerColor));
		//players[playerId].playerObjId = playerObjId;
		//worldModel.addPlayerObj(playerId, playerObjId, playerPos);
		
		// if this is me
		bool isMe = (playerId == this->playerId);

		planet.addPlayerObj(playerId, playerColor, playerPos, playerAimPos, isMe, tick);
		
		if (isMe)
		{
			// set ammo supply			
			(planet.getPlayerObjs())[playerId]->setAmmoSupply(static_cast<int>(playerPos.x + playerPos.y));
		}
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

};
