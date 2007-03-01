#include "ServerWorldModel.h"
#include "basic.h"
#include "common.h"
#include <cmath>
#include <limits>

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
		playerObjs.add(playerId, new PlayerObj(playerPos, 1, getTimeHandler()->getTick()));
	}

	GameObjId ServerWorldModel::addObstacle(const Rectangle &obstacleArea)
	{
		//GameObjId obstacleId = obstacles.findFreeId();
		GameObjId obstacleId = getIdGenerator()->generateGameObjId();
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
		return obstacleId;
	}

	void ServerWorldModel::updatePlayerObjMovements(float deltaTime)
	{
		MovePlayerObj move(&getObstacles(), deltaTime, moveAlignedToAngle);
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), move);
	}

	void ServerWorldModel::updateProjectileMovements(float deltaTime, ServerPlayers &players)
	{
		MoveProjectile move(&obstacles, &players, *this, &(getPlayerObjs()), &respawnPoss, deltaTime);
		ForEach(projectiles.begin(), projectiles.end(), move);
		
		if (move.getProjectilesHit().size() > 0)
		{
			//std::cout << "---- number of projectiles to remove: " << move.getProjectilesHit().size() << std::endl;
			std::vector<RemoveProjectile>::iterator it = move.getProjectilesHit().begin();
			std::vector<RemoveProjectile>::iterator end = move.getProjectilesHit().end();
			for(; it != end ; ++it)
			{
				projectiles.remove(it->projectileId);
				//std::cout << "\tremoving projectile: " << it->projectileId << std::endl;
				
				// send message			
				pushMessageToAll(players, *it, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}

			//std::cout << "\tprojectiles left: " << projectiles.getSize() << std::endl;
			//ProjectileContainer::Iterator it2 = projectiles.begin();
			//ProjectileContainer::Iterator it2End = projectiles.end();
			//for(; it2 != it2End; ++it2)
			//{
			//	std::cout << "\tprojectile left: " << it2->first << std::endl;
			//}
		}
	}



	void ServerWorldModel::MoveProjectile::operator ()(const ProjectileContainer::Pair &projectilePair)
	{
		assert(players); // must be able to send updates
		assert(obstacles);
		assert(playerObjs);
		
		// move projectile
		Projectile *projectile = projectilePair.second;
		Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed() * deltaTime);
		projectile->setPos(projectile->getPos() + moveVec);
		Line projectileLine(projectile->getLine());


		// Hit collision, find hit point

		float minHitDist = 2.0f;			
		GameObjId obstacleIdHit;

		ObstacleContainer::Iterator obstacleIt = obstacles->begin();
		ObstacleContainer::Iterator obstacleEnd = obstacles->end();		
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

		PlayerObjContainer::Iterator playerObjIt = playerObjs->begin();
		PlayerObjContainer::Iterator playerObjEnd = playerObjs->end();		
		for(; playerObjIt != playerObjEnd; ++playerObjIt)
		{
			if (playerObjIt->first != static_cast<GameObjId>(projectile->getShooterId())) // cannot hit the shooter itself
			{
				Rectangle rectangle;
				playerObjIt->second->getRectangle(rectangle);
				float localMinHitDist = projectileLine.minCrossPoint(rectangle);
				if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
				{
					hitPlayerObj = true;
					minHitDist = localMinHitDist;
					playerIdHit = playerObjIt->first;
				}
			}
		}
		

		
		if (minHitDist <= 1.0f) // did hit any object?
		{
			Pos hitPos(projectileLine.getPosAlong(minHitDist));

			//// debug render hit pos
			//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			//Rectangle crossBox(hitPos, 15.0f);	
			//WorldRenderer::renderRectangle(crossBox, GL_QUADS);

			// Calculate and apply damage to playerobjects
			playerObjIt = playerObjs->begin();
			playerObjEnd = playerObjs->end();		
			for(; playerObjIt != playerObjEnd; ++playerObjIt)
			{
				GameObjId targetPlayerId = playerObjIt->first;
				PlayerObj *playerObj = playerObjIt->second;

				// calculate damage
				int damage;
				if (hitPlayerObj && (targetPlayerId == playerIdHit))
				{
					// direct hit damage
					damage = projectile->getDirectDamage();
				}
				else
				{
					// blast damage
					damage = projectile->getBlastDamage(minHitDist, playerObj->getPos());
				}
				
				// apply damage to playerobject
				if (damage > 0)
				{
					playerObj->hurt(damage);	
					if (playerObj->isDead())
					{
						// player was kiled
						PlayerId killerId = projectile->getShooterId();
						//++((*playerObjs)[killerId]->frags);
						
						// produce an unpredictable respawn place
						Pos tmpPos = playerObj->pos + projectile->getPos();
						size_t respawnPosId = static_cast<size_t>(abs(playerObj->health + static_cast<int>(tmpPos.x + tmpPos.y))) % respawnPoss->size();
						
						Pos &respawnPos = (*respawnPoss)[respawnPosId];						
						playerObj->respawn(respawnPos);

						// send kill message to all players
						Kill kill(killerId, targetPlayerId, respawnPos);
						pushMessageToAll(*players, kill, getTimeHandler()->getTime(), getTimeHandler()->getTick());
					}
				}
			}

			// remove projectile later
			RemoveProjectile removeProjectile(projectilePair.first, hitPos);
			projectilesHit.push_back(removeProjectile);
			
		}
		
	}

	GameObjId ServerWorldModel::playerShoot(PlayerId playerId, Projectile::Type weapon)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());
		Angle angle = playerObj->angle;

		//GameObjId projectileId = getProjectiles().findFreeId();
		GameObjId projectileId = getIdGenerator()->generateGameObjId();
		getProjectiles().add(projectileId, new Projectile(weapon, pos, angle, playerId, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick()));

		return projectileId;
	}
};

