#include "Client.h"
#include "Player.h"
//#include <iostream>
#include <string>
#include <algorithm>

//// solving LNK error
//#include "WorldRenderer.cpp"

namespace Prototype
{

	const double Client::OBJECT_LAG_MODIFIER = 1.2;
	const int Client::OBJECT_LAG_ADD_TIME = 18;
	const int Client::OBJECT_LAG_ADD_TICK = 1;

	Client::Client() : ClientGlobalAccess(&clientGlobalObj), worldModel(&clientGlobalObj), worldRenderer(WorldRenderer::FOLLOW_PLAYER),
						connectionPhase(0), requestRender(false), currentObjLag(0)
	{
		predictionHandler.setWorldModel(&worldModel);
		assert(predictionHandler.isConsistent());
	}

	Client::~Client()
	{
		ClientPlayers::Iterator it(players.begin());
		ClientPlayers::Iterator end(players.end());
		for(; it != end; ++it)
		{
			delete it->second;
		}
	}

	//void Client::handleEvents()
	//{
		//if (aimMode == KEYBOARD)
		//{
		//	PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

		//	kh.isPressed(CMD_ROTATE_LEFT);
		//	kh.isReleased(CMD_ROTATE_LEFT);
		//	if (kh.isDown(CMD_ROTATE_LEFT))
		//	{
		//		playerObj->angle += 0.1f;	
		//	}

		//	kh.isPressed(CMD_ROTATE_RIGHT);
		//	kh.isReleased(CMD_ROTATE_RIGHT);
		//	if (kh.isDown(CMD_ROTATE_RIGHT))
		//	{
		//		playerObj->angle -= 0.1f;	
		//	}
		//}
	//}

	//void Client::logic()
	//{
	//	timeHandler.nextStep();
	//	
	//	assert(worldModel.isConsistent());
	//	if (timeHandler.isNewTick())
	//	{
	//		link.retrieve(timeHandler.getTime());
	//		//printf("ServerToClientLag = ");
	//		//printf(link.getCurrentLag());
	//		//printf("\n");
	//		std::cout << "ServerToClientLag = " << link.getCurrentLag() << "\n";

	//		// Read messages from server
	//		while(link.hasMessageOnQueue())
	//		{
	//			int messageType = link.popMessage();
	//			switch(messageType)
	//			{
	//			case UPDATE_PLAYER_OBJ:
	//				{
	//					UpdatePlayerObj *updatePlayerObj = link.getPoppedData<UpdatePlayerObj>();
	//					
	//					//PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
	//					PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
	//					//playerObj->pos = updatePlayerObj->pos;
	//					//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
	//					

	//					//Angle tmpAngle = playerObj->angle;
	//					//std::cout << "n_history_ticks" << CLIENT_PREDICTION_N_HISTORY_TICKS << std::endl;
	//					playerObj->setTickData(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle);
	//					if (playerId == updatePlayerObj->playerId)
	//					{
	//						//playerObj->angle = tmpAngle;

	//						// Use this PlayerObj Update message for Prediction.
	//						// Do not predict to this tick yet, we havn't collected the user input for this input yet.
	//						int predictToTick = static_cast<int>(timeHandler.getStepTick()) - 1;
	//						predictionHandler.predict(playerId, link.getPoppedTick(), predictToTick);
	//					}
	//						
	//					//playerObj->pos = updatePlayerObj->pos;
	//					//if (playerId != updatePlayerObj->playerId)
	//					//{
	//					//	playerObj->angle = updatePlayerObj->angle;
	//					//}
	//					//playerObj->storeToTickData(
	//				}
	//				break;
	//			case ADD_PLAYER_OBJ:
	//				{
	//					AddPlayerObj *addPlayerObj = link.getPoppedData<AddPlayerObj>();
	//					addPlayer(addPlayerObj->playerId, addPlayerObj->color, addPlayerObj->pos, link.getPoppedTick());				

	//					if (connectionPhase == ClientPhase::GET_ADDPLAYEROBJ)
	//						connectionPhase++;
	//				}
	//				break;
	//			case ADD_OBSTACLE:
	//				{
	//					AddObstacle *addObstacle = link.getPoppedData<AddObstacle>();
	//					worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
	//					
	//					if (connectionPhase == ClientPhase::GET_ADDOBSTACLE)
	//						connectionPhase++; 
	//				}
	//				break;
	//			case ADD_PROJECTILE:
	//				{
	//					printf("CLIENT: handling add_projectile @ %d\n", timeHandler.getTime());

	//					AddProjectile *addProjectile = link.getPoppedData<AddProjectile>();
	//					worldModel.addProjectile(addProjectile->projectileId, static_cast<Projectile::Type>(addProjectile->type), addProjectile->pos, addProjectile->angle, addProjectile->shooterId, link.getPoppedTick());
	//				}
	//				break;
	//			case UPDATE_PROJECTILE:
	//				{
	//					UpdateProjectile *updateProjectile = link.getPoppedData<UpdateProjectile>();
	//					Projectile *projectile = (worldModel.getProjectiles())[updateProjectile->projectileId];
	//					//projectile->setPos(updateProjectile->pos);
	//					projectile->setUpdateData(link.getPoppedTick(), projectile->getPos());
	//					
	//				}
	//				break;
	//			case REMOVE_PROJECTILE:
	//				{
	//					RemoveProjectile *removeProjectile = link.getPoppedData<RemoveProjectile>();
	//					worldRenderer.projectileHit((worldModel.getProjectiles())[removeProjectile->projectileId], removeProjectile->hitPosition);
	//					worldModel.getProjectiles().remove(removeProjectile->projectileId);					
	//				}
	//				break;
	//			case KILL:
	//				{
	//					Kill *kill = link.getPoppedData<Kill>();
	//					PlayerObj *killer = (worldModel.getPlayerObjs())[kill->killerId];
	//					PlayerObj *killed = (worldModel.getPlayerObjs())[kill->killedId];
	//					killed->pos = kill->respawnPos;
	//					killed->setAmmoSupply(static_cast<int>(killed->pos.x + killed->pos.y + killer->pos.x + killer->pos.y));
	//					if (killed->getAmmoCurrentWeapon() <= 0) killed->switchWeapon();
	//				}
	//				break;
	//			case START_GAME:
	//				//timeHandler.reset();
	//				break;
	//			default:
	//				break;
	//			};


	//		}

	//		//printf("CONNECTION PHASE: %d\n", connectionPhase);

	//		if (connectionPhase == ClientPhase::RUNNING)
	//		{
	//			handleEvents();
	//			int time = timeHandler.getStepTime();

	//			//if (kh.isPressed(CMD_SHOOT))
	//			//{
	//			//	ShootCmd shootCmd = {playerId};
	//			//	
	//			//	link.pushMessage(shootCmd);
	//			//	link.transmit();
	//			//}

	//			PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

	//			//if (kh.isPressed(CMD_SWITCH_WEAPON))
	//			//{
	//			//	playerObj->switchWeapon();
	//			//}
	//			
	//			bool wasKeyEvent = kh.changePressedToDownState() || kh.changeReleasedToUpState();

	//			// handle shooting
	//			while (userInput.hasActionCmdOnQueue())
	//			{
	//				int actionCmd = userInput.popActionCmd();
	//			
	//				if (actionCmd == Cmds::SHOOT)
	//				{					
	//					if (playerObj->canShoot(time))
	//					{
	//						Projectile::Type weapon = playerObj->getCurrentWeapon();
	//						playerObj->shoot(time);
	//						ShootCmd shootCmd(playerId, weapon);
	//						link.pushMessage(shootCmd, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
	//					}
	//				}
	//				if (actionCmd == Cmds::SWITCH_WEAPON)
	//				{
	//					playerObj->switchWeapon();
	//				}
	//			}
	//			
	//			// If some key was pressed or released send message
	//			//if (wasKeyEvent || (this->mousePosChanged && (this->aimMode == MOUSE)))
	//			if (!kh.isDown(CMD_SWITCH_WEAPON))
	//			{
	//				if (this->mousePosChanged && (this->aimMode == MOUSE))
	//				{
	//					updatePlayerObjAngle();
	//					this->mousePosChanged = false;
	//				}

	//				

	//				int stateCmds = getUserInput()->getCurrentStates().getStates();
	//				float aimangle = ((worldModel.getPlayerObjs())[playerId])->angle.getFloat();
	//				
	//				UserCmd userCmd(stateCmds, aimangle);
	//				//printf("CLIENT: sending usercmd with tick: %f @ time %d\n", timeHandler.getStepTick(), timeHandler.getStepTime());
	//				link.pushMessage(userCmd, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
	//				predictionHandler.setUserCmd(userCmd, static_cast<int>(timeHandler.getStepTick()));
	//				
	//			}

	//			// perform final prediction
	//			predictionHandler.predict(playerId, static_cast<int>(timeHandler.getStepTick()) + 1);

	//			// transmit any messages
	//			link.transmit();
	//		}
	//	}
	//}

	void Client::getCurrentUserCmd(UserCmd &userCmd)
	{
		//static int lastTick = -100;
		//assert(timeHandler.getStepTick() >= (lastTick +1));
		//lastTick = timeHandler.getStepTick();

		// get current tick
		int currentTick = static_cast<int>(getStepTick());
		
		// get playerobj
		PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
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

		// get angle
		Angle aimAngle(0.0f);
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			if (userInputHandler.aimMode == UserInputHandler::MOUSE)
			{
				aimAngle = calcPlayerObjAngle(userInputHandler.getMouseScreenPos());
			}
			else
			{
				assert(userInputHandler.aimMode == UserInputHandler::KEYBOARD);
				//playerAngle = (worldModel.getPlayerObjs())[playerId]->angle;				
				Angle preAngle(preUserCmd.aimAngle);
				aimAngle = calcPlayerObjAngle(preUserCmd.aimAngle, stateCmds);
			}
		}

		// set stateCmds, aimAngle and firstProjectileId
		userCmd.stateCmds = stateCmds;
		userCmd.aimAngle = aimAngle;
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
			else
			{
				assert(false);
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
					
					//PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
					PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
					//playerObj->pos = updatePlayerObj->pos;
					//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
					

					//Angle tmpAngle = playerObj->angle;
					//std::cout << "n_history_ticks" << CLIENT_PREDICTION_N_HISTORY_TICKS << std::endl;
					
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
						//playerObj->angle = tmpAngle;

						// Use this PlayerObj Update message for Prediction.						
						
						//predictionHandler.predict(playerId, link.getPoppedTick(), predictToTick);						
					}
					else
					{
						playerObj->setTickData(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
					}
						
					//playerObj->pos = updatePlayerObj->pos;
					//if (playerId != updatePlayerObj->playerId)
					//{
					//	playerObj->angle = updatePlayerObj->angle;
					//}
					//playerObj->storeToTickData(
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
					//projectile->setPos(updateProjectile->pos);
					projectile->setTickData(link.getPoppedTick(), updateProjectile->pos);
					//projectile->setPos(updateProjectile->pos);
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
				{
					//timeHandler.reset();
					
				}
				break;
			case SET_TICK_0_TIME:
				getTimeHandler()->enterTick0Time((link.getPoppedData<SetTick0Time>())->tick0Time);
				//std::cout << "tick0Time: " << timeHandler.getTick0Time() << std::endl;
				break;
			default:
				break;
			};
		}

		//if (connectionPhase == ClientPhase::RUNNING)
		//{
		//	// start
		//	UserCmd startUserCmd;
		//	startUserCmd.clear();
		//	timeHandler.nextStep();				
		//	predictionHandler.setUserCmd(startUserCmd, static_cast<int>(timeHandler.getStepTick()));
		//}
	}

	void Client::runStep()
	{
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

				//std::cout << nowRunning << " runstep: " << currentTick << std::endl;
			}
			else
			{
				requestRender = true;
				//std::cout << "wants to render" << std::endl;
			}
		}
		else
		{
			handleServerMessages();
		}
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
		worldRenderer.setupProjection();
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			Tickf currentTickf = getStepTick();
			Tickf playerObjRenderTickf = currentTickf - static_cast<Tickf>(currentObjLag);			
			
			
			worldModel.updatePlayerObjsToTickData(playerObjRenderTickf);
			(worldModel.getPlayerObjs())[playerId]->updateToTickData(currentTickf);


			
			//WorldModel::PlayerObjs::Iterator playerObjIt = worldModel.getPlayerObjs().begin();
			//WorldModel::PlayerObjs::Iterator playerObjEnd = worldModel.getPlayerObjs().end();
			//for(; it != end; ++it)
			//{
			//	it->second->updateToTickData(tick);
			//}
			//(worldModel.getPlayerObjs())[playerId]->updateToTickData(currentTickf);

			WorldModel::Projectiles::Iterator projectilesIt = worldModel.getProjectiles().begin();
			WorldModel::Projectiles::Iterator projectilesEnd = worldModel.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{
				//std::cout << "projectile at client!" << std::endl;
				
				Projectile *projectile = projectilesIt->second;
				Tickf projectileRenderTickf;
				if (projectile->getShooterId() == static_cast<GameObjId>(playerId))
				{
					projectileRenderTickf = currentTickf;
				}
				else
				{
					projectileRenderTickf = Projectile::RENDER_LAG_MODS.getRenderTick(currentTickf, projectile, currentObjLag);					
				}

				projectile->updateToTickData(projectileRenderTickf);
				projectile->render = (projectileRenderTickf >= projectile->getShootTick());

				
			}


			worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerId]);//, timeHandler.getStepTick());
		}
		requestRender = false;

		//update
		handleServerMessages();


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

	void Client::addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick)
	{
		
		//size_t playerId = players.getSize();
		//size_t playerObjId = playerId; // for now
		
		players.add(playerId, new Player(playerColor));
		//players[playerId].playerObjId = playerObjId;
		//worldModel.addPlayerObj(playerId, playerObjId, playerPos);
		
		// if this is me
		bool isMe = (playerId == this->playerId);

		worldModel.addPlayerObj(playerId, playerPos, isMe, tick);
		
		
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

		//link.setSimulatedRecieveLag(2);
	}

	KeyHandler* Client::getKeyHandler()
	{
		return &kh;
	}

	//void Client::setCurrentMousePos(Vec2<int> mouseScreenPos)
	//{
	//	mousePosChanged = true;
	//	if (worldModel.getPlayerObjs().exists(playerId))
	//	{
	//		PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
	//		
	//		viewportHandler.renderArea.size = WorldRenderer::RENDER_SIZE; // set current render area
	//		viewportHandler.renderArea.setCenterPos(playerObj->pos); // set current render area
	//		mousePos = viewportHandler.screenToGame(mouseScreenPos); // mouse position in game
	//	}
	//}

	Angle Client::calcPlayerObjAngle(Vec2<int> mouseScreenPos)
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
			Pos mousePos(viewportHandler.screenToGame(worldRenderer.getRenderArea(playerObj), mouseScreenPos));
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
			//playerObj->angle = angle;
			return angle;

		}
		return 0.0f;
	}

	Angle Client::calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds)
	{
		//PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

		bool left = stateCmds.getCurrentState(Cmds::ROTATE_LEFT);
		bool right = stateCmds.getCurrentState(Cmds::ROTATE_RIGHT);
		float rotateAmount(PlayerObj::getRotateSpeed());
		Angle deltaAngle((left ? rotateAmount : 0.0f) + (right ? -rotateAmount : 0.0f));
		return preAngle + deltaAngle;		
	}

};
