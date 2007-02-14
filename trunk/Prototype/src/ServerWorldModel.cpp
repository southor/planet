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

	void ServerWorldModel::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		playerObjs.add(playerId, new PlayerObj(playerPos));
	}

	size_t ServerWorldModel::addObstacle(const Rectangle &obstacleArea)
	{
		size_t obstacleId = obstacles.findFreeId();
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
		return obstacleId;
	}

	void ServerWorldModel::updatePlayerObjMovements(float deltaTime)
	{
		Move move(&obstacles, *this, deltaTime, moveAlignedToAngle);
		ForEach(playerObjs.begin(), playerObjs.end(), move);
	}

	void ServerWorldModel::updateProjectileMovements(float deltaTime, ServerPlayers &players)
	{
		Move move(&obstacles, &players, *this, &(getPlayerObjs()), &respawnPoss, deltaTime);
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
				pushMessageToAll(players, *it, getTimeHandler()->getTime());
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

	Obstacle* ServerWorldModel::Move::findAnyOverlap(const Rectangle &rectangle)
	{
		assert(obstacles);
		ServerObstacleContainer::Iterator it = obstacles->begin();
		ServerObstacleContainer::Iterator end = obstacles->end();		
		for(; it != end; ++it)
		{
			Obstacle *obstacle = it->second;
			if (obstacle->overlapping(rectangle)) return obstacle;
		}
		return NULL;
	}

	void ServerWorldModel::Move::operator ()(const PlayerObjContainer::Pair &playerObjPair)
	{
		float fbMoveDistance = deltaTime * PlayerObj::FORWARD_BACKWARD_SPEED;
		float strafeMoveDistance = deltaTime * PlayerObj::STRAFE_SPEED;

		// ----- produce a movevector from current actions and angle of playerObj ------

		PlayerObj *playerObj = playerObjPair.second;
		float moveAngle;
		if (moveAlignedToAngle) moveAngle = playerObj->angle;
		else moveAngle = PI/2.0f;
		Vec moveVec(0.0f, 0.0f);
		if (playerObj->movingForward == true)
		{
			moveVec += Vec(cos(moveAngle) * fbMoveDistance, sin(moveAngle) * fbMoveDistance);
		}
		if (playerObj->movingBackward == true)
		{
			moveVec += Vec(cos(moveAngle + PI) * fbMoveDistance, sin(moveAngle + PI) * fbMoveDistance);
		}
		if (playerObj->strafingLeft == true)
		{
			moveVec += Vec(cos(moveAngle + PI/2.0f) * strafeMoveDistance, sin(moveAngle + PI/2.0f) * strafeMoveDistance);
		}
		if (playerObj->strafingRight == true)
		{
			moveVec += Vec(cos(moveAngle - PI/2.0f) * strafeMoveDistance, sin(moveAngle - PI/2.0f) * strafeMoveDistance);
		}

		Vec zeroVec(0.0f, 0.0f);		
		if (moveVec != zeroVec)
		{
			// ----- fix any collisions with obstacles ------
			Vec usedMoveVec(moveVec);
			
			Rectangle playerRectangle;
			playerObj->getRectangle(playerRectangle);
			assert(findAnyOverlap(playerRectangle) == 0);

			Rectangle tmpRectangle(playerRectangle);			
			Obstacle *obstacle;
		
			tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
			obstacle = findAnyOverlap(tmpRectangle);

			if (obstacle)
			{
				usedMoveVec.x = 0.0f;
				tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
				
				if (obstacle->overlapping(tmpRectangle))
				{
					usedMoveVec.x = moveVec.x;
					usedMoveVec.y = 0.0f;
					tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
					
					if (findAnyOverlap(tmpRectangle))
					{
						usedMoveVec = zeroVec;
					}
				}
				else if (findAnyOverlap(tmpRectangle))
				{
					usedMoveVec = zeroVec;
				}
			}

			// finally move player
			playerObj->pos += usedMoveVec;
		}
	}

	void ServerWorldModel::Move::operator ()(const ProjectileContainer::Pair &projectilePair)
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
		size_t obstacleIdHit;

		ServerObstacleContainer::Iterator obstacleIt = obstacles->begin();
		ServerObstacleContainer::Iterator obstacleEnd = obstacles->end();		
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
		size_t playerIdHit;

		ServerPlayerObjContainer::Iterator playerObjIt = playerObjs->begin();
		ServerPlayerObjContainer::Iterator playerObjEnd = playerObjs->end();		
		for(; playerObjIt != playerObjEnd; ++playerObjIt)
		{
			if (playerObjIt->first != projectile->getShooterId()) // cannot hit the shooter itself
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
				size_t targetPlayerId = playerObjIt->first;
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
						size_t killerId = projectile->getShooterId();
						//++((*playerObjs)[killerId]->frags);
						
						// produce an unpredictable respawn place
						Pos tmpPos = playerObj->pos + projectile->getPos();
						size_t respawnPosId = static_cast<size_t>(abs(playerObj->health + static_cast<int>(tmpPos.x + tmpPos.y))) % respawnPoss->size();
						
						Pos &respawnPos = (*respawnPoss)[respawnPosId];						
						playerObj->respawn(respawnPos);

						// send kill message to all players
						Kill kill(killerId, targetPlayerId, respawnPos);
						pushMessageToAll(*players, kill, getTimeHandler()->getTime());
					}
				}
			}

			// remove projectile later
			RemoveProjectile removeProjectile(projectilePair.first, hitPos);
			projectilesHit.push_back(removeProjectile);
			
		}
		
	}

	size_t ServerWorldModel::playerShoot(size_t playerId, Projectile::Type weapon)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());
		float angle = playerObj->angle;

		size_t projectileId = getProjectiles().findFreeId();
		getProjectiles().add(projectileId, new Projectile(weapon, pos, angle, playerId));

		return projectileId;
	}
};

