#include "serverworldmodel.h"
#include "basic.h"
#include <cmath>

namespace Prototype
{
	ServerWorldModel::~ServerWorldModel()			
	{
		deleteAllObjs();
	}

	size_t ServerWorldModel::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		return playerObjs.add(new PlayerObj(playerId, playerPos));
	}

	void ServerWorldModel::updatePlayerObjMovements()
	{
		Move move(&obstacles);
		ForEach(playerObjs.begin(), playerObjs.end(), move);
	}

	Obstacle* ServerWorldModel::Move::findAnyOverlap(const Rectangle &rectangle)
	{
		ServerObstacleContainer::Iterator it;
		ServerObstacleContainer::Iterator end;		
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
			moveVec += Vec(sin(angle - PI/2.0f) * 3.0f, cos(angle - PI/2.0f) * 3.0f);
		}
		if (playerObj->strafingRight == true)
		{
			moveVec += Vec(sin(angle + PI/2.0f) * 3.0f, cos(angle + PI/2.0f) * 3.0f);
		}

		Vec zeroVec(0.0f, 0.0f);
		if (moveVec != zeroVec)
		{
			// ----- fix any collisions with obstacles ------
			
			Rectangle playerRectangle;
			playerObj->getRectangle(playerRectangle);
			assert(findAnyOverlap(playerRectangle) == NULL);

			Rectangle tmpRectangle;
			Obstacle *obstacle;
		
			tmpRectangle.pos = playerRectangle.pos + moveVec;
			obstacle = findAnyOverlap(tmpRectangle);

			if (obstacle)
			{
				moveVec.x = 0.0f;
				tmpRectangle.pos = playerRectangle.pos + moveVec;
				
				if (obstacle->overlapping(tmpRectangle))
				{
					moveVec.y = 0.0f;
				}
				else
				{
					obstacle = findAnyOverlap(tmpRectangle);
					if (obstacle) moveVec.y = 0.0f;
				}
			}

			// finally move player
			playerObj->pos += moveVec;
		}
	}
};