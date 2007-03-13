#include "Client.h"
//#include "Player.h"
#include <string>
#include <algorithm>

namespace Planet
{
	//const double Client::OBJECT_LAG_MODIFIER = 1.2;
	//const int Client::OBJECT_LAG_ADD_TIME = 18;
	//const int Client::OBJECT_LAG_ADD_TICK = 1;

	Client::Client() : planetBody(5.0f) {}

	void Client::init()
	{
		planetBody.init();
		ship.setPlanet(&planetBody);
		sight.setCamera(&camera);
		sight.setPlanet(&planetBody);
	}

	//Client::Client() : worldRenderer(WorldRenderer::FOLLOW_PLAYER),
	//					connectionPhase(0), requestRender(false), currObjLag(0)
	//{
	//	predictionHandler.setWorldModel(&worldModel);
	//	assert(predictionHandler.isConsistent());
	//}

	Client::~Client()
	{
	//	ClientPlayers::Iterator it(players.begin());
	//	ClientPlayers::Iterator end(players.end());
	//	for(; it != end; ++it)
	//	{
	//		delete it->second;
	//	}
	}

	//void Client::getCurrentUserCmd(UserCmd &userCmd)
	//{
	//	// get current tick
	//	int currentTick = static_cast<int>(timeHandler.getStepTick());
	//	
	//	// get playerobj
	//	PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
	//	playerObj->updateToTickData(currentTick);
	//	if (!playerObj)
	//	{
	//		userCmd.clear();
	//		return;
	//	}

	//	// get previous user command
	//	UserCmd preUserCmd;
	//	predictionHandler.getUserCmd(preUserCmd, currentTick - 1);

	//	// get userCmd to start with (could contain postponed actionCmds from previous tick)
	//	predictionHandler.getUserCmd(userCmd, currentTick);

	//	
	//	// get stateCmds
	//	StateCmds stateCmds = userInputHandler.getCurrentStates();

	//	// get angle
	//	Angle aimAngle(0.0f);
	//	if (worldModel.getPlayerObjs().exists(playerId))
	//	{
	//		if (userInputHandler.aimMode == UserInputHandler::MOUSE)
	//		{
	//			aimAngle = calcPlayerObjAngle(userInputHandler.getMouseScreenPos());
	//		}
	//		else
	//		{
	//			assert(userInputHandler.aimMode == UserInputHandler::KEYBOARD);
	//			//playerAngle = (worldModel.getPlayerObjs())[playerId]->angle;				
	//			Angle preAngle(preUserCmd.aimAngle);
	//			aimAngle = calcPlayerObjAngle(preUserCmd.aimAngle, stateCmds);
	//		}
	//	}

	//	// set stateCmds and aimAngle
	//	userCmd.stateCmds = stateCmds;
	//	userCmd.aimAngle = aimAngle;

	//	// set shooting action and nShots
	//	assert(userCmd.nShots == 0);
	//	userCmd.firstShotTick = playerObj->getNextShootTick();
	//	//if (userCmd.shootAction == UserCmd::CONTINUE_SHOOTING)		
	//	if (userCmd.isShooting())
	//	{			
	//		assert(userCmd.firstShotTick >= static_cast<Tickf>(currentTick));
	//		userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick);
	//	}
	//	//else
	//	//{
	//	//	assert(userCmd.shootAction == UserCmd::NOT_SHOOTING);
	//	//}
	//	while(userInputHandler.hasActionCmdOnQueue())
	//	{
	//		int actionCmd = userInputHandler.popActionCmd();

	//		if ((userCmd.nShots > 0) || userCmd.isShooting())
	//		{
	//			if (actionCmd == Cmds::STOP_SHOOTING)
	//			{
	//				//userCmd.shootAction = UserCmd::NOT_SHOOTING;
	//				userCmd.shooting = false;
	//				userCmd.nShots = tmin(1, userCmd.nShots);
	//				std::cout << "    -- stop shooting" << std::endl;
	//			}
	//			else if (actionCmd == Cmds::SWITCH_WEAPON)
	//			{
	//				UserCmd nextUserCmd;
	//				userCmd.assumeNext(nextUserCmd);
	//				nextUserCmd.weapon = playerObj->getNextWeapon(userCmd.weapon);
	//				predictionHandler.setUserCmd(nextUserCmd, currentTick + 1);
	//				break;
	//			}
	//		}
	//		else
	//		{
	//			if (actionCmd == Cmds::START_SHOOTING)
	//			{
	//				//userCmd.shootAction = UserCmd::START_SHOOTING;
	//				userCmd.shooting = true;
	//				userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick);
	//				//continuous = false;
	//				std::cout << "    -- start shooting" << std::endl;
	//			}
	//			else if (actionCmd == Cmds::SWITCH_WEAPON)
	//			{
	//				userCmd.weapon = playerObj->getNextWeapon(userCmd.weapon);
	//			}
	//		}
	//	}

	//	if (userCmd.isShooting() || (userCmd.nShots > 0))
	//	{			
	//		//userCmd.nShots = playerObj->getNTickShots(userCmd.weapon, currentTick, userCmd.shootAction == UserCmd::CONTINUE_SHOOTING);
	//		std::cout << "-- " << currentTick << " shooting, nShots = " << userCmd.nShots;
	//		std::cout << "   playerObj.nextShootTick  = " << playerObj->getNextShootTick() << std::endl;
	//	}
	//	
	//	assert(userCmd.isConsistent(currentTick));
	//}

	//void Client::handleServerMessages()
	//{
	//	link.retrieve(timeHandler.getTime());
	//	
	//	while(link.hasMessageOnQueue())
	//	{
	//		int messageType = link.popMessage();
	//		switch(messageType)
	//		{
	//		case UPDATE_PLAYER_OBJ:
	//			{
	//				UpdatePlayerObj *updatePlayerObj = link.getPoppedData<UpdatePlayerObj>();
	//				
	//				//PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerObjId];
	//				PlayerObj *playerObj = (worldModel.getPlayerObjs())[updatePlayerObj->playerId];
	//				//playerObj->pos = updatePlayerObj->pos;
	//				//printf("CLIENT: updating client position to: %f, %f\n", playerObj->pos.x, playerObj->pos.y);
	//				

	//				//Angle tmpAngle = playerObj->angle;
	//				//std::cout << "n_history_ticks" << CLIENT_PREDICTION_N_HISTORY_TICKS << std::endl;
	//				
	//				if (playerId == updatePlayerObj->playerId)
	//				{
	//					bool differ = playerObj->setTickDataAndCompare(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
	//					if (differ)
	//					{
	//						//std::cout << "old prediction differ!" << std::endl;
	//						predictionHandler.serverInput(playerId, link.getPoppedTick());						
	//					}
	//					else
	//					{
	//						//std::cout << "old prediction ok!" << std::endl;
	//					}
	//					//playerObj->angle = tmpAngle;

	//					// Use this PlayerObj Update message for Prediction.						
	//					
	//					//predictionHandler.predict(playerId, link.getPoppedTick(), predictToTick);						
	//				}
	//				else
	//				{
	//					playerObj->setTickData(link.getPoppedTick(), updatePlayerObj->pos, updatePlayerObj->angle, updatePlayerObj->nextShootTick);
	//				}
	//					
	//				//playerObj->pos = updatePlayerObj->pos;
	//				//if (playerId != updatePlayerObj->playerId)
	//				//{
	//				//	playerObj->angle = updatePlayerObj->angle;
	//				//}
	//				//playerObj->storeToTickData(
	//			}
	//			break;
	//		case ADD_PLAYER_OBJ:
	//			{
	//				AddPlayerObj *addPlayerObj = link.getPoppedData<AddPlayerObj>();
	//				addPlayer(addPlayerObj->playerId, addPlayerObj->color, addPlayerObj->pos, link.getPoppedTick());				

	//				if (connectionPhase == ClientPhase::GET_ADDPLAYEROBJ)
	//					connectionPhase++;
	//			}
	//			break;
	//		case ADD_OBSTACLE:
	//			{
	//				AddObstacle *addObstacle = link.getPoppedData<AddObstacle>();
	//				worldModel.addObstacle(addObstacle->obstacleId, addObstacle->obstacleArea);
	//				
	//				if (connectionPhase == ClientPhase::GET_ADDOBSTACLE)
	//					connectionPhase++; 
	//			}
	//			break;
	//		case START_GAME:
	//			break;
	//		case SET_TICK_0_TIME:
	//			timeHandler.enterTick0Time(*(link.getPoppedData<SetTick0Time>()));
	//			//std::cout << "tick0Time: " << timeHandler.getTick0Time() << std::endl;
	//			break;
	//		default:
	//			break;
	//		};
	//	}
	//}

	void Client::runStep()
	{
		ship.moveUp = userInputHandler.getCurrentState(Cmds::TMP_UP);
		ship.moveDown = userInputHandler.getCurrentState(Cmds::TMP_DOWN);
		ship.moveLeft = userInputHandler.getCurrentState(Cmds::TMP_LEFT);
		ship.moveRight = userInputHandler.getCurrentState(Cmds::TMP_RIGHT);
			
	
		// Logic
		ship.logic();

		camera.update(ship.position, ship.reference);
		//sight.update(userInputHandler.getMouseScreenPos(), WINDOW_SIZE.x, WINDOW_SIZE.y);

		ship.direction = sight.position - ship.position;
		
	//	assert(isConsistent());
	//	
	//	timeHandler.nextStep();
	//	if (connectionPhase == ClientPhase::RUNNING)
	//	{
	//		if (timeHandler.isNewTick())
	//		{
	//			assert(predictionHandler.isConsistent());
	//			
	//			int currentTick = static_cast<int>(timeHandler.getStepTick());
	//			

	//			// calculate current objectLag				
	//			double tmp = static_cast<double>(link.getCurrentLag() - timeHandler.getTick0Time());
	//			this->currObjLag = static_cast<int>(tmp * OBJECT_LAG_MODIFIER + OBJECT_LAG_ADD_TIME)
	//												/ TimeHandler::TICK_DELTA_TIME + OBJECT_LAG_ADD_TICK;
	//			//std::cout << "currObjLag: " << currObjLag << std::endl;

	//			// get userCmd
	//			UserCmd userCmd;
	//			getCurrentUserCmd(userCmd);
	//			
	//			// store userCmd
	//			predictionHandler.setUserCmd(userCmd, currentTick);
	//			
	//			// push userCmd to serverlink
	//			link.pushMessage(userCmd, timeHandler.getTime(), currentTick);

	//			// transmit messages
	//			link.transmit();

	//			// perform prediction
	//			predictionHandler.predict(playerId, currentTick + 1);
	//		}
	//		else
	//		{
	//			requestRender = true;
	//		}
	//	}
	//	else
	//	{
	//		handleServerMessages();
	//	}
	}

	//bool Client::initConnection()
	//{
	//	if (connectionPhase == ClientPhase::SEND_INITCLIENT)
	//	{
	//		// send init package to server
	//		InitClient initClient = InitClient(color);
	//		link.pushMessage(initClient, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
	//		link.transmit();

	//		connectionPhase++;
	//	}

	//	if (connectionPhase == ClientPhase::WAIT_WELCOME_CLIENT)
	//	{
	//		link.retrieve(timeHandler.getTime());
	//		if (link.hasMessageOnQueue())
	//		{
	//			int messageType = link.popMessage();
	//			if (messageType == WELCOME_CLIENT)
	//			{
	//				WelcomeClient *welcomeClient = link.getPoppedData<WelcomeClient>();
	//			
	//				setPlayerId(welcomeClient->playerId);

	//				connectionPhase++;
	//			}
	//			else
	//			{
	//				assert(false);
	//			}
	//		}
	//	}

	//	if (connectionPhase == ClientPhase::SYNC_SEND_PING)
	//	{
	//		// send ping to server with current client time
	//		SyncPing syncPing(playerId, timeHandler.getTime());
	//		link.pushMessage(syncPing, timeHandler.getTime(), static_cast<int>(timeHandler.getStepTick()));
	//		link.transmit();

	//		connectionPhase++;
	//	}

	//	if (connectionPhase == ClientPhase::SYNC_GET_PONG)
	//	{
	//		// get pong pack from server with server time and time when ping was sent
	//		link.retrieve(timeHandler.getTime());
	//		if (link.hasMessageOnQueue())
	//		{
	//			int messageType = link.popMessage();
	//			if (messageType == SYNC_PONG)
	//			{
	//				SyncPong *syncPong = link.getPoppedData<SyncPong>();
	//				
	//				int clientTime = timeHandler.getTime();
	//				int serverTime = syncPong->time;

	//				int pingTime = clientTime - syncPong->pingSendTime;
	//				int serverClientDiff = (serverTime + pingTime/2) - clientTime;

	//				printf("Adjusting client time with diff: %d\n", serverClientDiff);

	//				// Modify client time to match server time
	//				timeHandler.incrementTime(serverClientDiff);
 //
	//				connectionPhase++;

	//				return true;
	//			}
	//			else
	//			{
	//				assert(false);
	//			}
	//		}
	//	}

	//	return false;
	//}

	void Client::renderAndUpdate()
	{
		//render
		planetBody.render();

		// Disable lights for ship and sight rendering
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		ship.render();
		sight.render();


		//worldRenderer.setupProjection();
		//if (worldModel.getPlayerObjs().exists(playerId))
		//{
		//	Tickf currentTick = timeHandler.getStepTick();
		//	worldModel.updateToTickData(currentTick - static_cast<Tickf>(currObjLag));
		//	(worldModel.getPlayerObjs())[playerId]->updateToTickData(currentTick);
		//	worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerId]);//, timeHandler.getStepTick());
		//}
		requestRender = false;

		//update
		//handleServerMessages();
	}

	//void Client::addPlayer(PlayerId playerId, const Color &playerColor, const Pos &playerPos, int tick)
	//{
	//	//size_t playerId = players.getSize();
	//	//size_t playerObjId = playerId; // for now
	//	
	//	players.add(playerId, new Player(playerColor));
	//	//players[playerId].playerObjId = playerObjId;
	//	//worldModel.addPlayerObj(playerId, playerObjId, playerPos);
	//	
	//	// if this is me
	//	bool isMe = (playerId == this->playerId);

	//	worldModel.addPlayerObj(playerId, playerPos, isMe, tick);
	//	
	//	if (isMe)
	//	{
	//		// set ammo supply
	//		(worldModel.getPlayerObjs())[playerId]->setAmmoSupply(static_cast<int>(playerPos.x + playerPos.y));
	//	}
	//}

	//void Client::setConnection(MessageSender *messageSender, MessageReciever *messageReciever)
	//{
	//	//this->messageSender = messageSender;
	//	//this->messageReciever = messageReciever;
	//	link.setMessageSender(messageSender);
	//	link.setMessageReciever(messageReciever);

	//	//link.setSimulatedRecieveLag(2);
	//}

	//KeyHandler* Client::getKeyHandler()
	//{
	//	return &kh;
	//}

	//Angle Client::calcPlayerObjAngle(Vec2<int> mouseScreenPos)
	//{
	//	//std::cout << mouseScreenPos.y << std::endl;
	//	
	//	if (worldModel.getPlayerObjs().exists(playerId))
	//	{
	//		PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
	//		
	//		//viewportHandler.renderArea.size = WorldRenderer::RENDER_SIZE; // set current render area
	//		//viewportHandler.renderArea.setCenterPos(playerObj->pos); // set current render area
	//		//Pos mousePos = viewportHandler.screenToGame(mouseScreenPos); // mouse position in game
	//		//std::cout << mousePos.y << std::endl;

	//		float angle;
	//		Pos mousePos(viewportHandler.screenToGame(worldRenderer.getRenderArea(playerObj), mouseScreenPos));
	//		Vec aimVec = mousePos - playerObj->pos;
	//		aimVec.normalize();
	//		if ((aimVec.x > 0.5) || (aimVec.x < 0.5f))
	//		{
	//			angle = asin(aimVec.y);
	//			if (aimVec.x < 0.0f) angle = PI_F - angle;
	//		}
	//		else
	//		{
	//			angle = acos(aimVec.x);
	//			if (aimVec.y < 0.0f) angle = -angle;
	//		}
	//		//playerObj->angle = angle;
	//		return angle;

	//	}
	//	return 0.0f;
	//}

	//Angle Client::calcPlayerObjAngle(Angle preAngle, StateCmds stateCmds)
	//{
	//	//PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];

	//	bool left = stateCmds.getCurrentState(Cmds::ROTATE_LEFT);
	//	bool right = stateCmds.getCurrentState(Cmds::ROTATE_RIGHT);
	//	float rotateAmount(PlayerObj::getRotateSpeed());
	//	Angle deltaAngle((left ? rotateAmount : 0.0f) + (right ? -rotateAmount : 0.0f));
	//	return preAngle + deltaAngle;		
	//}

};
