#include "basic.h"
#include "Client.h"
#include "Player.h"

#include <string>
#include <algorithm>

//// solving LNK error
//#include "WorldRenderer.cpp"

namespace Prototype
{
	const double Client::OBJECT_LAG_MODIFIER_DEFAULT = 1.2;
	const int Client::OBJECT_LAG_ADD_TIME_DEFAULT = 18;
	const int Client::OBJECT_LAG_ADD_TICK_DEFAULT = 2;

	Client::Client() : ClientGlobalAccess(&clientGlobalObj), worldModel(&clientGlobalObj), worldRenderer(WorldRenderer::FOLLOW_PLAYER),
						connectionPhase(0), requestRender(false), currentObjLag(0), predictionHandler(0)
	{
		if (configHandler.loadFile(CONFIG_FILENAME))
		{
			std::cout << "Client: using config file for game parameters" << std::endl;
		}
		else
		{
			std::cout << "* * * Client: Config file not found, using default values!!! * * *" << std::endl;
		}
	}

	Client::~Client()
	{
		ClientPlayers::Iterator it(players.begin());
		ClientPlayers::Iterator end(players.end());
		for(; it != end; ++it)
		{
			delete it->second;
		}

		delete predictionHandler;
	}

	void Client::getCurrentUserCmd(UserCmd &userCmd)
	{
		assert(predictionHandler);

		// get current tick
		int currentTick = static_cast<int>(getStepTick());
		
		// get playerobj
		PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
		playerObj->updateToTickData(currentTick);
		if (!playerObj)
		{
			userCmd.clear(playerId, currentTick);
			return;
		}

		// get previous user command
		UserCmd preUserCmd;		
		predictionHandler->getUserCmd(preUserCmd, currentTick - 1);

		// get userCmd to start with (could contain postponed actionCmds from previous tick)
		predictionHandler->getUserCmd(userCmd, currentTick);		
		
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
		if (userCmd.isShooting())
		{			
			assert(userCmd.firstShotTick >= static_cast<Tickf>(currentTick));
			userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick);
		}

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
					predictionHandler->setUserCmd(nextUserCmd, currentTick + 1);
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

		
		assert(userCmd.isConsistent(playerId, currentTick));
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
					
					PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
					
					if (playerId == updatePlayerObj->playerId)
					{
						bool differ = playerObj->setTickDataAndCompare(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
						if (differ)
						{
							std::cout << "old prediction differ!" << std::endl;
							assert(predictionHandler);
							predictionHandler->serverInput(link.getPoppedTick());
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
					AddProjectile *addProjectile = link.getPoppedData<AddProjectile>();					
					bool projectileAlreadyExists= false;
					if (static_cast<PlayerId>(addProjectile->shooterId) == playerId)
					{
						projectileAlreadyExists = worldModel.getProjectiles().exists(addProjectile->projectileId);
					}
					
					if (projectileAlreadyExists)
					{
						assert((configHandler.getIntValue("client_create_projectile", CLIENT_CREATE_PROJECTILE_DEFAULT) == 1)
								&& (addProjectile->shooterId == static_cast<GameObjId>(playerId)));
					}
					else if (addProjectile->shooterId != static_cast<GameObjId>(playerId))
					{
						assert(static_cast<int>(addProjectile->shootTick) == link.getPoppedTick());
						worldModel.addProjectile(addProjectile->projectileId, static_cast<Projectile::Type>(addProjectile->type), addProjectile->pos, addProjectile->angle, addProjectile->shooterId, addProjectile->shootTick, addProjectile->objLag);
					}
				}
				break;
			case UPDATE_PROJECTILE:
				{
					UpdateProjectile *updateProjectile = link.getPoppedData<UpdateProjectile>();
					
					WorldModel::Projectiles::Iterator it = (worldModel.getProjectiles()).find(updateProjectile->projectileId);
					
					if (it != (worldModel.getProjectiles()).end())
					{
						Projectile *projectile = it->second;
						assert(projectile);
						if (projectile) projectile->setTickData(link.getPoppedTick(), updateProjectile->pos);
					}
					else
					{
						assert(configHandler.getIntValue("client_remove_projectile", CLIENT_REMOVE_PROJECTILE_DEFAULT) == 1);
					}
				}
				break;
			case REMOVE_PROJECTILE:
				{
					RemoveProjectile *removeProjectile = link.getPoppedData<RemoveProjectile>();
					bool removed = worldModel.getProjectiles().remove(removeProjectile->projectileId);
					assert(removed || configHandler.getIntValue("client_remove_projectile", CLIENT_REMOVE_PROJECTILE_DEFAULT) == 1);
				}
				break;
			case PROJECTILE_HIT:
				{
					ProjectileHit *projectileHit = link.getPoppedData<ProjectileHit>();
					
					WorldModel::Projectiles::Iterator it = (worldModel.getProjectiles()).find(projectileHit->projectileId);
					
					if (it != (worldModel.getProjectiles()).end())
					{
						Projectile *projectile = it->second;
						projectile->setHit(link.getPoppedTick(), projectileHit->hitPosition);
					}
					else
					{
						assert(configHandler.getIntValue("client_remove_projectile", CLIENT_REMOVE_PROJECTILE_DEFAULT) == 1);
					}
				}
				break;
			case START_GAME:
				{
				}
				break;
			case SET_TICK_0_TIME:
				getTimeHandler()->enterTick0Time((link.getPoppedData<SetTick0Time>())->tick0Time);
				break;
			default:
				break;
			};
		}

		predictionHandler->rePredictIfNeeded();
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
				if ((static_cast<int>(getTimeHandler()->getStepTick()) % (6000/TimeHandler::TICK_DELTA_TIME)) == 0)
				{
					debugPrintState();
				}
				
				assert(predictionHandler->isConsistent());
				
				int currentTick = static_cast<int>(getStepTick());
				
				// calculate current objectLag
				this->currentObjLag = calcCurrentObjLag();

				// get userCmd
				UserCmd userCmd;
				getCurrentUserCmd(userCmd);
				userCmd.objLag = this->currentObjLag; // send currentObjLag to server
				
				// store userCmd
				predictionHandler->setUserCmd(userCmd, currentTick);
				
				// push userCmd to serverlink
				link.pushMessage(userCmd, getTimeHandler()->getTime(), currentTick);

				// transmit messages
				link.transmit();

				//perform shooting
				PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
				playerObj->updateToTickData(currentTick);
				playerObj->setUserCmd(&userCmd);
				worldModel.handlePlayerShooting(playerId, configHandler.getIntValue("client_create_projectile", CLIENT_CREATE_PROJECTILE_DEFAULT) == 1);

				// perform prediction
				predictionHandler->predict(currentTick + 1);

				// remove projectiles that have hit something
				std::vector<GameObjId> projectileRemoves;
				WorldModel::Projectiles::Iterator projectilesIt(worldModel.getProjectiles().begin());
				WorldModel::Projectiles::Iterator projectilesEnd(worldModel.getProjectiles().end());
				for(; projectilesIt != projectilesEnd; ++projectilesIt)
				{
					GameObjId projectileId = projectilesIt->first;
					Projectile *projectile = projectilesIt->second;
					
					// predict hits of own projectiles
					if ((projectile->getShooterId() == static_cast<GameObjId>(playerId)) && configHandler.getIntValue("client_remove_projectile", CLIENT_REMOVE_PROJECTILE_DEFAULT) == 1)
					{
						projectile->updateToTickData(currentTick - 1);
						
						ProjectileHit projectileHit(projectileId, Pos(0.0f, 0.0f));						
						if (worldModel.performProjectileHit(projectileId, projectileHit))
						{
							projectile->setHit(currentTick, projectileHit.hitPosition);
						}
					}
					
					// check hits and remove them later
					int checkTick = currentTick;
					if (projectile->getShooterId() != static_cast<GameObjId>(playerId)) checkTick -= this->currentObjLag;					
					if (projectile->getHitTick() <= checkTick)
						projectileRemoves.push_back(projectileId);
				}
				for(size_t removeId = 0; removeId < projectileRemoves.size(); ++removeId)
				{
					projectileHit(projectileRemoves[removeId]);
				}
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
	}

	int Client::calcCurrentObjLag() const
	{
		// get modifiying values
		double objLagModifier = configHandler.getDoubleValue("object_lag_modifier", OBJECT_LAG_MODIFIER_DEFAULT);
		int objLagAddTime = configHandler.getIntValue("object_lag_time", OBJECT_LAG_ADD_TIME_DEFAULT);
		int objLagAddTick = configHandler.getIntValue("object_lag_tick", OBJECT_LAG_ADD_TICK_DEFAULT);
		
		// get base objLag (can in some cases be negative if the result of syncronise of clocks wasn't perfect)
		double baseObjLagTime = static_cast<double>(link.getCurrentLag() - getTimeHandler()->getTick0Time());

		// multiply depending on positive or negative
		if (baseObjLagTime > 0.0) baseObjLagTime *= objLagModifier;
		else baseObjLagTime /= objLagModifier;

		// add extra modifiers and change to "ticks"
		int currentObjLag = static_cast<int>(baseObjLagTime + objLagAddTime) /
								TimeHandler::TICK_DELTA_TIME + objLagAddTick;
		
		// do not allow negative objLag
		return tmax(currentObjLag, 0);
	}

	void Client::projectileHit(GameObjId projectileId)
	{
		Projectile *projectile = (worldModel.getProjectiles())[projectileId];
		worldRenderer.projectileHit(projectile,
									projectile->getHitPos());
		bool removed = worldModel.getProjectiles().remove(projectileId);
		assert(removed);
		//if (removed) std::cout << "projectile removed" << std::endl;
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
					predictionHandler = new PredictionHandler(welcomeClient->playerId, &worldModel);
					assert(predictionHandler->isConsistent());

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

			WorldModel::Projectiles::Iterator projectilesIt = worldModel.getProjectiles().begin();
			WorldModel::Projectiles::Iterator projectilesEnd = worldModel.getProjectiles().end();
			for(; projectilesIt != projectilesEnd; ++projectilesIt)
			{
				Projectile *projectile = projectilesIt->second;
				assert(projectile);
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
	}

	void Client::addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick)
	{
		players.add(playerId, new Player(playerColor));
		
		// if this is me
		bool isMe = (playerId == this->playerId);

		worldModel.addPlayerObj(playerId, playerPos, isMe, tick, configHandler.getIntValue("player_obj_health", PlayerObj::HEALTH_DEFAULT));
		
		if (isMe)
		{
			// set ammo supply			
			(worldModel.getPlayerObjs())[playerId]->setAmmoSupply(static_cast<int>(playerPos.x + playerPos.y));
		}
	}

	void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	{
		link.setMessageSender(messageSender);
		link.setMessageReciever(messageReciever);
	}

	KeyHandler* Client::getKeyHandler()
	{
		return &kh;
	}

	Angle Client::calcPlayerObjAngle(Vec2<int> mouseScreenPos)
	{
		if (worldModel.getPlayerObjs().exists(playerId))
		{
			PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

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

			return angle;
		}
		
		return 0.0f;
	}

	Angle Client::calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds)
	{
		bool left = stateCmds.getCurrentState(Cmds::ROTATE_LEFT);
		bool right = stateCmds.getCurrentState(Cmds::ROTATE_RIGHT);
		float rotateAmount(PlayerObj::getRotateSpeed());
		Angle deltaAngle((left ? rotateAmount : 0.0f) + (right ? -rotateAmount : 0.0f));
		return preAngle + deltaAngle;		
	}

	void Client::debugPrintState()
	{
		std::cout << "Client tick: " << static_cast<int>(getTimeHandler()->getStepTick());
		std::cout << "     Number of projectiles: " << worldModel.getProjectiles().getSize() << std::endl;
	}

};
