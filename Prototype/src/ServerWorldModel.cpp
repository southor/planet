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

	void ServerWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	{
		PlayerObj *playerObj = new PlayerObj(playerPos, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick());
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
			Projectile *projectile = projectileIt->second;
			
			// move projectile
			Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed());
			projectile->setPos(projectile->getPos() + moveVec);
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

	void ServerWorldModel::performProjectileHits(ServerPlayers &players)
	{

		std::vector<ProjectileHit> projectilesHit;
		
		Projectiles::Iterator projectileIt(getProjectiles().begin());
		Projectiles::Iterator projectileEnd(getProjectiles().end());
		for(; projectileIt != projectileEnd; ++projectileIt)
		{
			// get projectile
			Projectile *projectile = projectileIt->second;
			Line projectileLine(projectile->getLine());


			// Hit collision, find hit point

			float minHitDist = 2.0f;			
			GameObjId obstacleIdHit;

			Obstacles::Iterator obstacleIt = getObstacles().begin();
			Obstacles::Iterator obstacleEnd = getObstacles().end();		
			for(; obstacleIt != obstacleEnd; ++obstacleIt)
			{
				float localMinHitDist =  projectileLine.minCrossPoint(*(obstacleIt->second));
				if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
				{
					minHitDist = localMinHitDist;
					obstacleIdHit = obstacleIt->first;
				}
			}

			bool hitPlayerObj = false;
			GameObjId playerIdHit;

			PlayerObjs::Iterator playerObjIt = getPlayerObjs().begin();
			PlayerObjs::Iterator playerObjEnd = getPlayerObjs().end();		
			for(; playerObjIt != playerObjEnd; ++playerObjIt)
			{
				GameObjId targetPlayerObjId = playerObjIt->first;
				PlayerObj *targetPlayerObj = playerObjIt->second;
				
				if (targetPlayerObjId != static_cast<GameObjId>(projectile->getShooterId())) // cannot hit the shooter itself
				{
					/*
					 * Update the playerObj to the object lag that the shooting
					 * client was using when the projectile was fired
					 */
					targetPlayerObj->updateToTickData(getTimeHandler()->getTick() - projectile->getObjLag());
				
					Rectangle rectangle;
					targetPlayerObj->getRectangle(rectangle);
					float localMinHitDist = projectileLine.minCrossPoint(rectangle);
					if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
					{
						hitPlayerObj = true;
						minHitDist = localMinHitDist;
						playerIdHit = targetPlayerObjId;
					}

					// Update the playerObj to the current tick again
					targetPlayerObj->updateToTickData(getTimeHandler()->getTick());
				}
				//else
				//{
				//	std::cout << "the shooter was not hit by himself" << std::endl;
				//}
			}
			

			
			if (minHitDist <= 1.0f) // did hit any object?
			{
				Pos hitPos(projectileLine.getPosAlong(minHitDist));

				//// debug render hit pos
				//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
				//Rectangle crossBox(hitPos, 15.0f);	
				//WorldRenderer::renderRectangle(crossBox, GL_QUADS);

				// Calculate and apply damage to playerobjects
				playerObjIt = getPlayerObjs().begin();
				playerObjEnd = getPlayerObjs().end();		
				for(; playerObjIt != playerObjEnd; ++playerObjIt)
				{
					GameObjId targetPlayerObjId = playerObjIt->first;
					PlayerObj *targetPlayerObj = playerObjIt->second;

					// Update the target playerObj to the object lag
					if (playerObjIt->first != static_cast<GameObjId>(projectile->getShooterId()))
					{
						targetPlayerObj->updateToTickData(getTimeHandler()->getTick() - projectile->getObjLag());
					}

					// calculate damage
					int damage;
					if (hitPlayerObj && (targetPlayerObjId == playerIdHit))
					{
						// direct hit damage
						damage = projectile->getDirectDamage();
					}
					else
					{
						// blast damage
						damage = projectile->getBlastDamage(minHitDist, targetPlayerObj->getPos());
					}

					// Update the playerObj back to the current tick again
					targetPlayerObj->updateToTickData(getTimeHandler()->getTick());
					
					// apply damage to playerobject
					if (damage > 0)
					{
						std::cout << "damage =  " << damage << std::endl;
						targetPlayerObj->hurt(damage);
						if (targetPlayerObj->isDead())
						{
							std::cout << "!!! a player was killed !!!  " << std::endl;

							// player was kiled
							PlayerId killerId = projectile->getShooterId();
							//++((*playerObjs)[killerId]->frags);
							
							// produce an unpredictable respawn place
							Pos tmpPos = targetPlayerObj->pos + projectile->getPos();
							size_t respawnPosId = static_cast<size_t>(abs(targetPlayerObj->health + static_cast<int>(tmpPos.x + tmpPos.y))) % respawnPoss.size();
							
							Pos &respawnPos = respawnPoss[respawnPosId];						
							targetPlayerObj->respawn(respawnPos);

							// send kill message to all players
							Kill kill(killerId, targetPlayerObjId, respawnPos);
							pushMessageToAll(players, kill, getTimeHandler()->getTime(), getTimeHandler()->getTick());
						}
					}
					//else if (targetPlayerId != projectile->getShooterId())
					//{
					//	std::cout << "no damage to an enemy" << std::endl;
					//}

					
				}

				// push projectile hit for later
				ProjectileHit projectileHit(projectileIt->first, hitPos);
				projectilesHit.push_back(projectileHit);
				//std::cout << "((((( projectile hit! )))))" << std::endl;
				
			}
		}

		if (projectilesHit.size() > 0)
		{
			//std::cout << "---- number of projectiles to remove: " << move.getProjectilesHit().size() << std::endl;
			std::vector<ProjectileHit>::iterator it = projectilesHit.begin();
			std::vector<ProjectileHit>::iterator end = projectilesHit.end();
			for(; it != end ; ++it)
			{
				assert(projectiles.remove(it->projectileId));
				//std::cout << "\tremoving projectile: " << it->projectileId << std::endl;
				
				// send message			
				pushMessageToAll(players, *it, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			//std::cout << "\tprojectiles left: " << projectiles.getSize() << std::endl;
			//Projectiles::Iterator it2 = projectiles.begin();
			//Projectiles::Iterator it2End = projectiles.end();
			//for(; it2 != it2End; ++it2)
			//{
			//	std::cout << "\tprojectile left: " << it2->first << std::endl;
			//}
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
		const UserCmd &userCmd(playerObj->getUserCmd());
		
		assert((userCmd.firstShotTick >= playerObj->getNextShootTick()) &&
				(playerObj->getNextShootTick() >= currentTickf));
		
		// Do any shooting
		int nShots = userCmd.nShots;
		Projectile::Type weapon = userCmd.weapon;
		for(int i=0; i<nShots; ++i)
		{
			
			//Tickf shootTick = playerObj->getShotTick(currentTick, i);
			//GameObjId projectileId = playerShoot(playerId, weapon, shootTick, userCmd.objLag);
			//Projectile *projectile = (getProjectiles())[projectileId];
			
			//GameObjId projectileId = getIdGenerator()->generateGameObjId();
			GameObjId projectileId = userCmd.firstProjectileId + i;
			if (playerTryShoot(playerId, currentTick, i, projectileId))
			{		
				Projectile *projectile = (getProjectiles())[projectileId];
				std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
				
				// send projectile to all clients
				AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
											projectile->getAngle().getFloat(), projectile->getShooterId(),
											projectile->getShootTick(), projectile->getObjLag());
				pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				//ServerPlayers::Iterator it = players.begin();
				//ServerPlayers::Iterator end = players.end();
				//for(; it != end; ++it)
				//{
				//	if (it->first != playerId) it->second->link.pushMessage(addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				//}
			}
			else
			{
				RemoveProjectile removeProjectile(projectileId);
				players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}
		}		
	}
};

