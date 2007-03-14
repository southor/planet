#include "Planet.h"
#include "Cmds.h"

namespace Planet
{
	const Vec Planet::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	void Planet::storeToTickData(int tick)
	{
		Ships::Iterator it = getShips().begin();
		Ships::Iterator end = getShips().end();
		for(; it != end; ++it)
		{
			//it->second->storeToTickData(tick);
			assert(false);
		}
	}

	void Planet::updateToTickData(int tick)
	{
		Ships::Iterator it = getShips().begin();
		Ships::Iterator end = getShips().end();
		for(; it != end; ++it)
		{
			//it->second->updateToTickData(tick);
			assert(false);
		}
	}

	void Planet::updateToTickData(Tickf tick)
	{
		Ships::Iterator it = getShips().begin();
		Ships::Iterator end = getShips().end();
		for(; it != end; ++it)
		{
			//it->second->updateToTickData(tick);
			assert(false);
		}
	}


	void Planet::deleteAllObjs()
	{
		{ // delete ships
			Ships::Iterator it = ships.begin();
			Ships::Iterator end = ships.end();
			for(; it != end; ++it)
			{
				assert(it->second);
				delete it->second;
			}
		}
		
		////Delete del;
		//DeleteObstacle deleteObstacle;
		//DeletePlayerObj deletePlayerObj;
		//DeleteProjectile deleteProjectile;
		//ForEach(getObstacles().begin(), getObstacles().end(), deleteObstacle);
		//ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), deletePlayerObj);
		//ForEach(getProjectiles().begin(), getProjectiles().end(), deleteProjectile);
	}

	//Obstacle* Planet::Move::findAnyOverlap(const Rectangle &rectangle)
	//{
	//	assert(obstacles);
	//	Obstacles::Iterator it = obstacles->begin();
	//	Obstacles::Iterator end = obstacles->end();		
	//	for(; it != end; ++it)
	//	{
	//		Obstacle *obstacle = it->second;
	//		if (obstacle->overlapping(rectangle)) return obstacle;
	//	}
	//	return NULL;
	//}

	//void Planet::MovePlayerObj::operator ()(const PlayerObjs::Pair &playerObjPair)
	//{
	//	float fbMoveDistance = PlayerObj::getForwardBackwardSpeed();
	//	float strafeMoveDistance = PlayerObj::getStrafeSpeed();

	//	// ----- produce a movevector from current actions and angle of playerObj ------

	//	PlayerObj *playerObj = playerObjPair.second;
	//	Angle moveAngle;
	//	if (moveAlignedToAngle) moveAngle = playerObj->angle;
	//	else moveAngle = PI_F/2.0f;
	//	Vec moveVec(0.0f, 0.0f);
	//	const StateCmds &stateCmds(playerObj->getUserCmd().stateCmds);
	//	if (stateCmds.getCurrentState(Cmds::FORWARD))
	//	{
	//		moveVec += Vec(cos(moveAngle.getFloat()) * fbMoveDistance, sin(moveAngle.getFloat()) * fbMoveDistance);
	//	}
	//	if (stateCmds.getCurrentState(Cmds::BACKWARD))
	//	{
	//		moveVec += Vec(cos((moveAngle + Angle::PI).getFloat()) * fbMoveDistance, sin((moveAngle + Angle::PI).getFloat()) * fbMoveDistance);
	//	}
	//	if (stateCmds.getCurrentState(Cmds::LEFT))
	//	{
	//		moveVec += Vec(cos((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance, sin((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance);
	//	}
	//	if (stateCmds.getCurrentState(Cmds::RIGHT))
	//	{
	//		moveVec += Vec(cos((moveAngle - Angle::PI/2.0f).getFloat()) * strafeMoveDistance, sin((moveAngle - Angle::PI/2.0f).getFloat()) * strafeMoveDistance);
	//	}

	//	Vec zeroVec(0.0f, 0.0f);		
	//	if (moveVec != zeroVec)
	//	{
	//		// ----- fix any collisions with obstacles ------
	//		Vec usedMoveVec(moveVec);
	//		
	//		Rectangle playerRectangle;
	//		playerObj->getRectangle(playerRectangle);
	//		assert(findAnyOverlap(playerRectangle) == 0);

	//		Rectangle tmpRectangle(playerRectangle);			
	//		Obstacle *obstacle;
	//	
	//		tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
	//		obstacle = findAnyOverlap(tmpRectangle);

	//		if (obstacle)
	//		{
	//			usedMoveVec.x = 0.0f;
	//			tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
	//			
	//			if (obstacle->overlapping(tmpRectangle))
	//			{
	//				usedMoveVec.x = moveVec.x;
	//				usedMoveVec.y = 0.0f;
	//				tmpRectangle.pos = playerRectangle.pos + usedMoveVec;
	//				
	//				if (findAnyOverlap(tmpRectangle))
	//				{
	//					usedMoveVec = zeroVec;
	//				}
	//			}
	//			else if (findAnyOverlap(tmpRectangle))
	//			{
	//				usedMoveVec = zeroVec;
	//			}
	//		}

	//		// finally move player
	//		playerObj->pos += usedMoveVec;
	//	}
	//}

	bool Planet::isConsistent()
	{
		return getShips().isConsistent();
				//getObstacles().isConsistent() &&
				//getPlayerObjs().isConsistent() &&
				//getProjectiles().isConsistent();

	}
};
