#include "ServerWorldModel.h"
#include "basic.h"
#include "common.h"
#include <cmath>
#include <limits>
//#include "assert.h"

// debugging
#include "WorldRenderer.h"

namespace Prototype
{
	ServerWorldModel::~ServerWorldModel()			
	{
		deleteAllObjs();
	}

	void ServerWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos, int fullHealth)
	{
		PlayerObj *playerObj = new PlayerObj(playerId, playerPos, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick(), fullHealth);
		//try
		//{
			playerObjs.add(playerId, playerObj);
		//}
		//catch (std::bad_alloc ba)
		//{
		//	std::cout << ba.what() << std::endl;
		//	throw ba;
		//}
	}

	GameObjId ServerWorldModel::addObstacle(const Rectangle &obstacleArea)
	{
		//GameObjId obstacleId = obstacles.findFreeId();
		GameObjId obstacleId = getIdGenerator()->generateGameObjId();
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
		return obstacleId;
	}

	void ServerWorldModel::updatePlayerObjMovements()
	{
		MovePlayerObj move(&getObstacles(), moveAlignedToAngle);
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), move);
	}

	void ServerWorldModel::updateProjectileMovements()
	{
		Projectiles::Iterator projectileIt(getProjectiles().begin());
		Projectiles::Iterator projectileEnd(getProjectiles().end());
		for(; projectileIt != projectileEnd; ++projectileIt)
		{
			//Projectile *projectile = projectileIt->second;
			//
			//// move projectile
			//Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed());
			//projectile->setPos(projectile->getPos() + moveVec);

			projectileIt->second->move();
		}
		
		
		//MoveProjectile move(&obstacles, &players, *this, &(getPlayerObjs()), &respawnPoss);
		//ForEach(projectiles.begin(), projectiles.end(), move);

		//if (move.getProjectilesHit().size() > 0)
		//{
		//	//std::cout << "---- number of projectiles to remove: " << move.getProjectilesHit().size() << std::endl;
		//	std::vector<RemoveProjectile>::iterator it = move.getProjectilesHit().begin();
		//	std::vector<RemoveProjectile>::iterator end = move.getProjectilesHit().end();
		//	for(; it != end ; ++it)
		//	{
		//		projectiles.remove(it->projectileId);
		//		//std::cout << "\tremoving projectile: " << it->projectileId << std::endl;
		//		
		//		// send message			
		//		pushMessageToAll(players, *it, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		//	}

		//	//std::cout << "\tprojectiles left: " << projectiles.getSize() << std::endl;
		//	//Projectiles::Iterator it2 = projectiles.begin();
		//	//Projectiles::Iterator it2End = projectiles.end();
		//	//for(; it2 != it2End; ++it2)
		//	//{
		//	//	std::cout << "\tprojectile left: " << it2->first << std::endl;
		//	//}
		//}
	}

	//bool ServerWorldModel::performProjectileHit(GameObjId projectileId, ServerPlayers &players, ProjectileHit &projectileHit)
	//{
	//	// get projectile
	//	
	//	Projectile *projectile = projectiles[projectileId];
	//		//projectileIt->second;

	//	assert(projectile);
	//	Line projectileLine(projectile->getLine());


	//	// Hit collision, find hit point

	//	float minHitDist = 2.0f;			
	//	GameObjId obstacleIdHit;

	//	Obstacles::Iterator obstacleIt = getObstacles().begin();
	//	Obstacles::Iterator obstacleEnd = getObstacles().end();		
	//	for(; obstacleIt != obstacleEnd; ++obstacleIt)
	//	{
	//		float localMinHitDist =  projectileLine.minCrossPoint(*(obstacleIt->second));
	//		if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
	//		{
	//			minHitDist = localMinHitDist;
	//			obstacleIdHit = obstacleIt->first;
	//		}
	//	}

	//	bool hitPlayerObj = false;
	//	GameObjId playerIdHit;

	//	PlayerObjs::Iterator playerObjIt = getPlayerObjs().begin();
	//	PlayerObjs::Iterator playerObjEnd = getPlayerObjs().end();		
	//	for(; playerObjIt != playerObjEnd; ++playerObjIt)
	//	{
	//		GameObjId targetPlayerObjId = playerObjIt->first;
	//		PlayerObj *targetPlayerObj = playerObjIt->second;
	//		
	//		if (targetPlayerObjId != static_cast<GameObjId>(projectile->getShooterId())) // cannot hit the shooter itself
	//		{
	//			///*
	//			// * Update the playerObj to the object lag that the shooting
	//			// * client was using when the projectile was fired
	//			// */
	//			//targetPlayerObj->updateToTickData(getTimeHandler()->getTick() - projectile->getObjLag());
	//		
	//			Rectangle rectangle;
	//			targetPlayerObj->getRectangle(rectangle);
	//			float localMinHitDist = projectileLine.minCrossPoint(rectangle);
	//			if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
	//			{
	//				hitPlayerObj = true;
	//				minHitDist = localMinHitDist;
	//				playerIdHit = targetPlayerObjId;
	//			}

	//			//// Update the playerObj to the current tick again
	//			//targetPlayerObj->updateToTickData(getTimeHandler()->getTick());
	//		}
	//	}
	//	

	//	
	//	if (minHitDist <= 1.0f) // did hit any object?
	//	{
	//		Pos hitPos(projectileLine.getPosAlong(minHitDist));

	//		// Calculate and apply damage to playerobjects
	//		playerObjIt = getPlayerObjs().begin();
	//		playerObjEnd = getPlayerObjs().end();		
	//		for(; playerObjIt != playerObjEnd; ++playerObjIt)
	//		{
	//			GameObjId targetPlayerObjId = playerObjIt->first;
	//			PlayerObj *targetPlayerObj = playerObjIt->second;

	//			//// Update the target playerObj to the object lag
	//			//if (playerObjIt->first != static_cast<GameObjId>(projectile->getShooterId()))
	//			//{
	//			//	targetPlayerObj->updateToTickData(getTimeHandler()->getTick() - projectile->getObjLag());
	//			//}

	//			// calculate damage
	//			int damage;
	//			if (hitPlayerObj && (targetPlayerObjId == playerIdHit))
	//			{
	//				// direct hit damage
	//				damage = projectile->getDirectDamage();
	//			}
	//			else
	//			{
	//				// blast damage
	//				damage = projectile->getBlastDamage(minHitDist, targetPlayerObj->getPos());
	//			}

	//			//// Update the playerObj back to the current tick again
	//			//targetPlayerObj->updateToTickData(getTimeHandler()->getTick());
	//			
	//			// apply damage to playerobject
	//			if (damage > 0)
	//			{
	//				std::cout << "damage =  " << damage << std::endl;
	//				targetPlayerObj->hurt(damage);
	//				if (targetPlayerObj->isDead())
	//				{
	//					std::cout << "!!! a player was killed !!!  " << std::endl;

	//					// player was kiled
	//					PlayerId killerId = projectile->getShooterId();
	//					//++((*playerObjs)[killerId]->frags);
	//					
	//					// produce an unpredictable respawn place
	//					Pos tmpPos = targetPlayerObj->pos + projectile->getPos();
	//					size_t respawnPosId = static_cast<size_t>(abs(targetPlayerObj->health + static_cast<int>(tmpPos.x + tmpPos.y))) % respawnPoss.size();
	//					
	//					Pos &respawnPos = respawnPoss[respawnPosId];						
	//					targetPlayerObj->respawn(respawnPos);

	//					// send kill message to all players
	//					Kill kill(killerId, targetPlayerObjId, respawnPos);
	//					pushMessageToAll(players, kill, getTimeHandler()->getTime(), getTimeHandler()->getTick());
	//				}
	//			}	
	//		}
	//		
	//		ProjectileHit projectileHitTmp(projectileId, hitPos);
	//		projectileHit = projectileHitTmp;

	//		return true;
	//	}

	//	return false;
	//}

	void ServerWorldModel::performProjectileHits(ServerPlayers &players, WorldRenderer *worldRenderer)
	{
		int currentTick = getTimeHandler()->getTick();
		
		
		std::vector<ProjectileHit> projectilesHit;

		Projectiles::Iterator projectileIt(getProjectiles().begin());
		Projectiles::Iterator projectileEnd(getProjectiles().end());
		for(; projectileIt != projectileEnd; ++projectileIt)
		{			
			Projectile *projectile = projectileIt->second;
			int objLag = projectile->getObjLag();

			// update the opponents and shooter to the correct tick
			if (objLag != 0)
			{
				updatePlayerObjsToTickData(currentTick - objLag);
				(getPlayerObjs())[projectile->getShooterId()]->updateToTickData(currentTick);
			}

			// test projectile hit
			ProjectileHit projectileHit(projectileIt->first, Pos(0.0f, 0.0f));			
			//if (performProjectileHit(projectileIt->first, players, projectileHit))
			if (performProjectileHit(projectileIt->first, projectileHit))
			{
				// push projectile hit for later
				projectilesHit.push_back(projectileHit);

				// render explosion at server
				assert(worldRenderer);
				worldRenderer->projectileHit(projectile, projectileHit.hitPosition);
			}			
		}

		// update playerObjs back to current tick
		updatePlayerObjsToTickData(currentTick);

		// perform killing (playerObjs have already been hurt)
		PlayerObjs::Iterator playerObjsIt(getPlayerObjs().begin());
		PlayerObjs::Iterator playerObjsEnd(getPlayerObjs().end());
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{
			PlayerObj *playerObj = playerObjsIt->second;
			if (playerObj->isDead())
			{
				std::cout << "!!! a player was killed !!!  " << std::endl;

				// player was kiled
				PlayerId killerId = playerObj->lastHurter;
				//++((*playerObjs)[killerId]->frags);
				
				// produce an unpredictable respawn place
				Pos tmpPos = playerObj->pos;
				size_t respawnPosId = static_cast<size_t>(abs(playerObj->health + static_cast<int>(tmpPos.x + tmpPos.y)) + getTimeHandler()->getTick()) % respawnPoss.size();
				
				Pos &respawnPos = respawnPoss[respawnPosId];						
				playerObj->respawn(respawnPos);

				// send kill message to all players
				Kill kill(killerId, playerObjsIt->first, respawnPos);
				pushMessageToAll(players, kill, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}
		}

		

		// send hits and remove projectiles
		if (projectilesHit.size() > 0)
		{
			//std::cout << "---- number of projectiles to remove: " << move.getProjectilesHit().size() << std::endl;
			std::vector<ProjectileHit>::iterator it = projectilesHit.begin();
			std::vector<ProjectileHit>::iterator end = projectilesHit.end();
			for(; it != end ; ++it)
			{
				//assert(projectiles.remove(it->projectileId));
				bool result = projectiles.remove(it->projectileId);
				assert(result);
				
				
				// send message			
				pushMessageToAll(players, *it, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				
			}
		}
	}



	//void ServerWorldModel::MoveProjectile::operator ()(const Projectiles::Pair &projectilePair)
	//{
	//	assert(players); // must be able to send updates
	//	assert(obstacles);
	//	assert(playerObjs);
	//	


	//	// move projectile
	//	Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed());
	//	projectile->setPos(projectile->getPos() + moveVec);
	//	
	//}

	void ServerWorldModel::handlePlayerShooting(PlayerId playerId, ServerPlayers &players)
	{	
		int currentTick = getTimeHandler()->getTick();
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		
		// Copy userCmd, we must have a seperate copy to alter while original is intact.
		UserCmd userCmd(playerObj->getUserCmd());

		ClientIdGenerator *clientIdGenerator = players[playerId]->getIdGenerator();
		int clientNShots = userCmd.nShots;
		
		assert(userCmd.firstShotTick >= currentTickf);
		assert(playerObj->getNextShootTick() >= currentTickf);

		// remove projectiles if client created projectiles which we did't notice before
		while (userCmd.firstProjectileId > clientIdGenerator->getNextId())
		{
			RemoveProjectile removeProjectile(clientIdGenerator->generateGameObjId());
			players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		}
		assert(userCmd.firstProjectileId == clientIdGenerator->getNextId());
		
		if (clientNShots > 0)
		{
			assert(userCmd.firstShotTick >= playerObj->getNextShootTick());
		
			// Do any shooting
			int serverNShots = 0;
			Tickf shotTick;
			Projectile::Type weapon = userCmd.weapon;
			for(int i=0; i<clientNShots; ++i)
			{
				GameObjId projectileId = clientIdGenerator->generateGameObjId();
				if (playerTryShoot(playerId, currentTick, i, projectileId, shotTick))
				{		
					if (serverNShots == 0) userCmd.firstShotTick = shotTick;
					
					Projectile *projectile = (getProjectiles())[projectileId];
					std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
					
					// send projectile to all clients
					AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
												projectile->getAngle().getFloat(), projectile->getShooterId(),
												projectile->getShootTick(), projectile->getObjLag());
					pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());

					++serverNShots;
				}
				else
				{					
					RemoveProjectile removeProjectile(projectileId);
					players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				}
			}

			userCmd.nShots = serverNShots;
			
			// Safe to overwrite PlayerObj.userCmd now, write changes
			playerObj->setUserCmd(&userCmd);
		}

	}
};

