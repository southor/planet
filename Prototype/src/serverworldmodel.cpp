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

	void ServerWorldModel::updatePlayerObjMovements()
	{
		Move move(&obstacles);
		ForEach(playerObjs.begin(), playerObjs.end(), move);
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
		// ----- produce a movevector from current actions and angle of playerObj ------

		PlayerObj *playerObj = playerObjPair.second;
		float angle = playerObj->angle;
		Vec moveVec(0.0f, 0.0f);
		if (playerObj->movingForward == true)
		{
			moveVec += Vec(sin(angle) * 5.0f, cos(angle) * 5.0f);
		}
		if (playerObj->movingBackward == true)
		{
			moveVec += Vec(sin(angle + PI) * 5.0f, cos(angle + PI) * 5.0f);
		}
		if (playerObj->strafingLeft == true)
		{
			moveVec += Vec(sin(angle + PI/2.0f) * 3.0f, cos(angle + PI/2.0f) * 3.0f);
		}
		if (playerObj->strafingRight == true)
		{
			moveVec += Vec(sin(angle - PI/2.0f) * 3.0f, cos(angle - PI/2.0f) * 3.0f);
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
};