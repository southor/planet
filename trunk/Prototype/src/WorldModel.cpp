#include "WorldModel.h"

namespace Prototype
{
	const Vec WorldModel::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	void WorldModel::storeToTickData(int tick)
	{
		PlayerObjContainer::Iterator it = getPlayerObjs().begin();
		PlayerObjContainer::Iterator end = getPlayerObjs().end();
		for(; it != end; ++it)
		{
			it->second->storeToTickData(tick);
		}
	}

	void WorldModel::updateToTickData(int tick)
	{
		PlayerObjContainer::Iterator it = getPlayerObjs().begin();
		PlayerObjContainer::Iterator end = getPlayerObjs().end();
		for(; it != end; ++it)
		{
			it->second->updateToTickData(tick);
		}
	}

	void WorldModel::updateToTickData(Tickf tick)
	{
		PlayerObjContainer::Iterator it = getPlayerObjs().begin();
		PlayerObjContainer::Iterator end = getPlayerObjs().end();
		for(; it != end; ++it)
		{
			it->second->updateToTickData(tick);
		}
	}





	void WorldModel::deleteAllObjs()
	{
		//Delete del;
		DeleteObstacle deleteObstacle;
		DeletePlayerObj deletePlayerObj;
		DeleteProjectile deleteProjectile;
		ForEach(getObstacles().begin(), getObstacles().end(), deleteObstacle);
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), deletePlayerObj);
		ForEach(getProjectiles().begin(), getProjectiles().end(), deleteProjectile);
	}

	Obstacle* WorldModel::Move::findAnyOverlap(const Rectangle &rectangle)
	{
		assert(obstacles);
		ObstacleContainer::Iterator it = obstacles->begin();
		ObstacleContainer::Iterator end = obstacles->end();		
		for(; it != end; ++it)
		{
			Obstacle *obstacle = it->second;
			if (obstacle->overlapping(rectangle)) return obstacle;
		}
		return NULL;
	}

	void WorldModel::MovePlayerObj::operator ()(const PlayerObjContainer::Pair &playerObjPair)
	{
		float fbMoveDistance = deltaTime * PlayerObj::FORWARD_BACKWARD_SPEED;
		float strafeMoveDistance = deltaTime * PlayerObj::STRAFE_SPEED;

		// ----- produce a movevector from current actions and angle of playerObj ------

		PlayerObj *playerObj = playerObjPair.second;
		Angle moveAngle;
		if (moveAlignedToAngle) moveAngle = playerObj->angle;
		else moveAngle = PI_F/2.0f;
		Vec moveVec(0.0f, 0.0f);
		if (playerObj->movingForward == true)
		{
			moveVec += Vec(cos(moveAngle.getFloat()) * fbMoveDistance, sin(moveAngle.getFloat()) * fbMoveDistance);
		}
		if (playerObj->movingBackward == true)
		{
			moveVec += Vec(cos((moveAngle + Angle::PI).getFloat()) * fbMoveDistance, sin((moveAngle + Angle::PI).getFloat()) * fbMoveDistance);
		}
		if (playerObj->strafingLeft == true)
		{
			moveVec += Vec(cos((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance, sin((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance);
		}
		if (playerObj->strafingRight == true)
		{
			moveVec += Vec(cos((moveAngle - Angle::PI/2.0f).getFloat()) * strafeMoveDistance, sin((moveAngle - Angle::PI/2.0f).getFloat()) * strafeMoveDistance);
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

	bool WorldModel::isConsistent()
	{
		return getObstacles().isConsistent() &&
				getPlayerObjs().isConsistent() &&
				getProjectiles().isConsistent();
	}
};
