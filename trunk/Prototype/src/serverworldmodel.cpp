#include "serverworldmodel.h"
#include "basic.h"
#include <cmath>

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
		Move move(&obstacles, deltaTime);
		ForEach(playerObjs.begin(), playerObjs.end(), move);
	}

	void ServerWorldModel::updateProjectileMovements(float deltaTime)
	{
		Move move(&obstacles, deltaTime);
		ForEach(projectiles.begin(), projectiles.end(), move);
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
		float angle = playerObj->angle;
		Vec moveVec(0.0f, 0.0f);
		if (playerObj->movingForward == true)
		{
			moveVec += Vec(cos(angle) * fbMoveDistance, sin(angle) * fbMoveDistance);
		}
		if (playerObj->movingBackward == true)
		{
			moveVec += Vec(cos(angle + PI) * fbMoveDistance, sin(angle + PI) * fbMoveDistance);
		}
		if (playerObj->strafingLeft == true)
		{
			moveVec += Vec(cos(angle + PI/2.0f) * strafeMoveDistance, sin(angle + PI/2.0f) * strafeMoveDistance);
		}
		if (playerObj->strafingRight == true)
		{
			moveVec += Vec(cos(angle - PI/2.0f) * strafeMoveDistance, sin(angle - PI/2.0f) * strafeMoveDistance);
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
		Projectile *projectile = projectilePair.second;
		Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed() * deltaTime);
		projectile->pos = projectile->pos + moveVec;

		//TODO hitcollision, removing projectile, damages etc.
	}

	size_t ServerWorldModel::playerShoot(size_t playerId)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());
		float angle = playerObj->angle;
		Projectile::Type type = Projectile::BULLET;

		size_t projectileId = getProjectiles().findFreeId();
		getProjectiles().add(projectileId, new Projectile(type, pos, angle));

		return projectileId;
	}
};
