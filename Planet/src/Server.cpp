#include "Server.h"
#include "common.h"

//// resolving LNK error
//#include "WorldRenderer.cpp"

namespace Planet
{
	//const double Server::PREDICTION_AMOUNT_MODIFIER = 1.1;
	//const int Server::PREDICTION_AMOUNT_ADD_TIME = 10;

	Server::Server() {}

	//Server::Server() : worldRenderer(WorldRenderer::FOLLOW_PLAYER), lastUpdateTime(0),
	//					ServerGlobalAccess(&serverGlobalObj), worldModel(&serverGlobalObj),
	//					requestRender(false)
	//{
	//	// outer walls
	//	static const float WALL_THICKNESS = 500.0f;
	//	Rectangle wall1(0.0f, -WALL_THICKNESS, Planet::WORLD_SIZE.x, WALL_THICKNESS);
	//	Rectangle wall2(0.0f, Planet::WORLD_SIZE.y, Planet::WORLD_SIZE.x, WALL_THICKNESS);
	//	Rectangle wall3(-WALL_THICKNESS, -WALL_THICKNESS, WALL_THICKNESS, Planet::WORLD_SIZE.y + WALL_THICKNESS*2.0f);
	//	Rectangle wall4(Planet::WORLD_SIZE.x, -WALL_THICKNESS, WALL_THICKNESS, Planet::WORLD_SIZE.y + WALL_THICKNESS*2.0f);
	//	worldModel.addObstacle(wall1);
	//	worldModel.addObstacle(wall2);
	//	worldModel.addObstacle(wall3);
	//	worldModel.addObstacle(wall4);

	//	Rectangle obstacleArea(50.0f, 70.0f, 150.0f, 80.0f);
	//	worldModel.addObstacle(obstacleArea);
	//	obstacleArea.pos.x = 300.0f;
	//	worldModel.addObstacle(obstacleArea);

	//	Pos respawnPos1(50.0f, 250.0f);
	//	Pos respawnPos2(250.0f, 200.0f);
	//	worldModel.addRespawnPos(respawnPos1);
	//	worldModel.addRespawnPos(respawnPos2);
	//}

	//Server::~Server()
	//{
	//	ServerPlayers::Iterator it(players.begin());
	//	ServerPlayers::Iterator end(players.end());
	//	for(;it != end; ++it)
	//	{
	//		delete it->second;
	//	}		
	//}

	//bool Server::clientConnected(MessageSender *messageSender, MessageReciever *messageReciever)
	//{
	//	// Temp Link used to simplify sending and retrieving messages during this connection phase.
	//	Link link(messageSender, messageReciever);

	//	link.retrieve(getTimeHandler()->getTime());
	//	if (link.hasMessageOnQueue())
	//	{
	//		int messageType = link.popMessage();
	//		if (messageType == INIT_CLIENT)
	//		{
	//			// retrieve InitClient message from client
	//			InitClient *initClient = link.getPoppedData<InitClient>();
	//			Color color = initClient->color;

	//			// add player to server
	//			PlayerId playerId = addClient(color, messageSender, messageReciever);

	//			Pos startPos(200.0f + playerId * 50.0f, 200.0f);
	//			addPlayerObj(playerId, startPos);

	//			// send WelcomeClient with playerId to client
	//			WelcomeClient welcomeClient = WelcomeClient(playerId);
	//			link.pushMessage(welcomeClient, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//			link.transmit();
	//		}
	//		else if (messageType == SYNC_PING)
	//		{
	//			// retrieve SyncPing from client
	//			SyncPing *syncPing = link.getPoppedData<SyncPing>();
	//			PlayerId playerId = syncPing->playerId;
	//			int pingSendTime = syncPing->pingSendTime;

	//			// send SyncPong to client
	//			SyncPong syncPong(getTimeHandler()->getTime(), pingSendTime);
	//			link.pushMessage(syncPong, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//			link.transmit();

	//			return true;
	//		}
	//		else
	//		{
	//			assert(false);
	//		}
	//	}
	//	return false; // no client did try to connect
	//}

	//// TODO: MAKE PRIVATE
	//PlayerId Server::addClient(Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
	//{
	//	//PlayerId playerId = players.findFreeId();
	//	PlayerId playerId = getIdGenerator()->generatePlayerId();
	//	players.add(playerId, new ServerPlayer(color, messageSender, messageReciever));
	//	return playerId;
	//}

	//// TODO: MAKE PRIVATE
	//void Server::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	//{
	//	worldModel.addPlayerObj(playerId, playerPos);
	//}

	//void Server::startGame()
	//{
	//	Planet::PlayerObjs::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
	//	Planet::PlayerObjs::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
	//	for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
	//	{
	//		PlayerId playerId = playerObjsIt->first;
	//		PlayerObj *playerObj = playerObjsIt->second;

	//		AddPlayerObj addPlayerObj(playerId, players[playerId]->color, playerObj->pos);

	//		pushMessageToAll(players, addPlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//	}
	//
	//	// TODO Send the hole worldmodel to clients, all players and everything
	//	ServerPlayers::Iterator playersIt;
	//	for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
	//	{			
	//		const ServerPlayer *player = playersIt->second;

	//		Planet::Obstacles::Iterator obstaclesIt = worldModel.getObstacles().begin();
	//		Planet::Obstacles::Iterator obstaclesEnd = worldModel.getObstacles().end();
	//		for(; obstaclesIt != obstaclesEnd; ++obstaclesIt)
	//		{
	//			GameObjId obstacleId = obstaclesIt->first;
	//			Obstacle *obstacle = obstaclesIt->second;
	//			
	//			AddObstacle addObstacle(obstacleId, *obstacle);
	//			player->link.pushMessage(addObstacle, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//		}

	//		player->link.transmit();
	//	}
	//}

	//void Server::logic()
	//{
	//	const int tick = getTimeHandler()->getTick();
	//	const int time = getTimeHandler()->getTime();
	//		
	//	if (tick == 0)
	//	{
	//		int tickFromTime = 10 + getTimeHandler()->getTickFromTime();
	//		getTimeHandler()->setTick(tickFromTime);
	//		printf("start tick: %d, time: %d\n", tickFromTime, getTimeHandler()->getTime());
	//	}

	//	bool waitingForClients = false;
	//	int latestTick = 10000000; // used for debugging

	//	// check if current tick is recieved from all clients, otherwise set waitingForClients to true
	//	ServerPlayers::Iterator playersIt;
	//	for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
	//	{
	//		PlayerId playerId = playersIt->first;
	//		ServerPlayer *player(playersIt->second);
	//		
	//		player->link.retrieve(getTimeHandler()->getTime());

	//		// set waitingForClients to true if player doesn't have current tick
	//		waitingForClients = waitingForClients || (player->link.getLatestTick() < tick);

	//		//printf("playerId: %d, latestTick: %d\n", playerId, player.link.getLatestTick());

	//		if (player->link.getLatestTick() < latestTick) // used for debugging
	//			latestTick = player->link.getLatestTick(); // used for debugging

	//		// Check tick timeout
	//		if (getTimeHandler()->getTickFromTimeWithTimeout() >= tick)      //if (time > lastUpdateTime + 100) //ServerTimeHandler::TICK_DELTA_TIME + ServerTimeHandler::WAIT_FOR_TICK_TIMEOUT)
	//		{
	//			if (SERVER_PRINT_NETWORK_DEBUG) printf("#################### TIMEOUT ######################\n");
	//			waitingForClients = false;
	//			break; // exit for loop
	//		}
	//	}

	//	if (!waitingForClients)
	//	{
	//		if (SERVER_PRINT_NETWORK_DEBUG) printf("run tick: %d, tickFromTime: %d, tickWithTO: %d, latest: %d @ %d\n", tick, getTimeHandler()->getTickFromTime(), getTimeHandler()->getTickFromTimeWithTimeout(), latestTick, time);

	//		lastUpdateTime = time;

	//		worldModel.isConsistent();

	//		// Read messages from clients
	//		ServerPlayers::Iterator playersIt;
	//		for (playersIt = players.begin(); playersIt != players.end(); ++playersIt)
	//		{			
	//			PlayerId playerId = playersIt->first;
	//			ServerPlayer *player(playersIt->second);

	//			player->link.retrieve(getTimeHandler()->getTime());

	//			while (player->link.hasMessageOnQueueWithTick(tick))
	//			{
	//				int messageType = player->link.popMessage();
	//				if (messageType == USER_CMD)
	//				{
	//					UserCmd *userCmd = player->link.getPoppedData<UserCmd>();
	//					
	//					//if (userCmd->shootAction == UserCmd::CONTINUE_SHOOTING)
	//					//{
	//					//	// check that privious really is START_SHOOTING or CONTINUE_SHOOTING
	//					//	UserCmd preUserCmd;
	//					//	player.getUserCmd(preUserCmd, getTimeHandler()->getTick() - 1);
	//					//	if (preUserCmd.shootAction == UserCmd::NOT_SHOOTING)
	//					//	{
	//					//		userCmd->shootAction = UserCmd::START_SHOOTING;
	//					//	}
	//					//}
	//					
	//					player->setUserCmd(*userCmd, player->link.getPoppedTick());
	//					
	//				}
	//				//else if (messageType == SHOOT_CMD)
	//				//{
	//				//	printf("SERVER: handling shoot_cmd @ %d\n", getTimeHandler()->getTime());

	//				//	// player shoots
	//				//	ShootCmd *shootCmd = player.link.getPoppedData<ShootCmd>();					
	//				//	GameObjId projectileId = worldModel.playerShoot(shootCmd->playerId, shootCmd->weapon);
	//				//	Projectile *projectile = (worldModel.getProjectiles())[projectileId];
	//				//	
	//				//	// send projectile to all clients
	//				//	AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
	//				//								projectile->getAngle().getFloat(), projectile->getShooterId());
	//				//	pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//				//}
	//			}

	//			PlayerObj *playerObj = (worldModel.getPlayerObjs())[playerId];
	//			playerObj->updateToTickData(getTimeHandler()->getTick());
	//			UserCmd userCmd;
	//			player->getUserCmd(userCmd, getTimeHandler()->getTick());
	//			userCmd.isConsistent(getTimeHandler()->getTick());
	//			playerObj->setUserCmd(&userCmd);

	//			// shooting
	//			if (userCmd.nShots > 0)
	//			{
	//				//if (userCmd.shootAction == UserCmd::START_SHOOTING) std::cout << "server got: start shooting, nShots: " << userCmd.nShots << std::endl;
	//				//else if (userCmd.shootAction == UserCmd::CONTINUE_SHOOTING) std::cout << "server got: continue shooting, nShots: " << userCmd.nShots << std::endl;
	//				//else std::cout << "server got: not shooting, nShots: " << userCmd.nShots << std::endl;
	//				std::cout << "server: nShots: " << userCmd.nShots << std::endl;
	//			}
	//			std::vector<GameObjId> shots;
	//			worldModel.handlePlayerShooting(playerId, shots);
	//			for(size_t i=0; i<shots.size(); ++i)
	//			{
	//				GameObjId projectileId = shots[i];
 //					Projectile *projectile = (worldModel.getProjectiles())[projectileId];
	//				
	//				// send projectile to all clients
	//				AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
	//											projectile->getAngle().getFloat(), projectile->getShooterId(), projectile->getShootTick());
	//				pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//			}

	//			// Update next shoot tick for currentTick + 1
	//			playerObj->updateNextShootTick(getTimeHandler()->getTick());
	//		}		

	//		// update movements of objects
	//		worldModel.updatePlayerObjMovements();
	//		worldModel.updateProjectileMovements(players);

	//		getTimeHandler()->nextTick();
	//		lastUpdateTime = time;

	//		// Send playerObj updates and store state to history, also send tick0Time
	//		Planet::PlayerObjs::Iterator playerObjsIt = worldModel.getPlayerObjs().begin();
	//		Planet::PlayerObjs::Iterator playerObjsEnd = worldModel.getPlayerObjs().end();
	//		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
	//		{
	//			

	//			//GameObjId playerObjId = playerObjsIt->first;
	//			PlayerId playerId = playerObjsIt->first;
	//			PlayerObj *playerObj = playerObjsIt->second;
	//			//UpdatePlayerObj updatePlayerObj(playerObjId, playerObj->pos, playerObj->angle);
	//			UpdatePlayerObj updatePlayerObj(playerId, playerObj->pos, playerObj->angle, playerObj->getNextShootTick(), playerObj->getAmmo());

	//			pushMessageToAll(players, updatePlayerObj, getTimeHandler()->getTime(), getTimeHandler()->getTick());

	//			//playerObj->isConsistent();
	//			playerObj->storeToTickData(getTimeHandler()->getTick());

	//			// Send tick0Time to client
	//			{
	//				Link &link = players[playerId]->link;
	//				double lag = static_cast<double>(link.getCurrentLag());
	//				int extraPredictionTime = static_cast<int>(lag * PREDICTION_AMOUNT_MODIFIER) + PREDICTION_AMOUNT_ADD_TIME;
	//				SetTick0Time tick0Time(-extraPredictionTime);
	//				link.pushMessage(tick0Time, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//			}
	//		}

	//		// Send projectile updates
	//		Planet::Projectiles::Iterator projectilesIt = worldModel.getProjectiles().begin();
	//		Planet::Projectiles::Iterator projectilesEnd = worldModel.getProjectiles().end();
	//		for(; projectilesIt != projectilesEnd; ++projectilesIt)
	//		{			
	//			Projectile *projectile = projectilesIt->second;
	//			
	//			UpdateProjectile updateProjectile(projectilesIt->first, projectile->getPos());
	//			
	//			pushMessageToAll(players, updateProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());

	//			//projectile->storeToTickData(getTimeHandler()->getTick());
	//		}

	//		

	//		transmitAll(players);
	//		
	//		requestRender = true;			
	//	}
	//}

	//void Server::render()
	//{
	//	worldRenderer.setupProjection();			
	//	PlayerId playerIdFollow(1);
	//	if (worldModel.getPlayerObjs().exists(playerIdFollow))
	//	{
	//		worldRenderer.render(worldModel, players, (worldModel.getPlayerObjs())[playerIdFollow]); //, static_cast<Tickf>(getTimeHandler()->getTick()));
	//	}
	//	requestRender = false;
	//}
	
};
