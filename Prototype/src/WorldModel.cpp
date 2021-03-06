#include "WorldModel.h"
#include "Cmds.h"

#include "ServerPlayers.h"

namespace Prototype
{
	const Vec WorldModel::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	void WorldModel::updatePlayerObjsToTickData(Tickf tick)
	{
		PlayerObjs::Iterator it = getPlayerObjs().begin();
		PlayerObjs::Iterator end = getPlayerObjs().end();
		for(; it != end; ++it)
		{
			it->second->updateToTickData(tick);
		}
	}

	void WorldModel::playerShoot(PlayerId playerId, Projectile::Type weapon, Tickf shootTick, int objLag, GameObjId projectileId)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());
		Angle angle = playerObj->angle;

		Projectile *projectile = new Projectile(weapon, pos, angle, playerId, 1, shootTick, objLag);		
		getProjectiles().add(projectileId, projectile);
	}

	bool WorldModel::playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId, Tickf &shotTick)
	{
		Tickf currentTickf = static_cast<Tickf>(currentTick);
		
		PlayerObj *playerObj = getPlayerObjs()[playerId];		

		shotTick = playerObj->getShotTick(currentTick, shotN);
		if ((shotTick >= currentTickf) && (playerObj->getAmmoCurrentWeapon() >= 0))
		{
			const UserCmd &userCmd(playerObj->getUserCmd());
			assert((shotN < userCmd.nShots) && (shotN >= 0));

			playerShoot(playerId, userCmd.weapon, shotTick, userCmd.objLag, projectileId);
			Projectile *projectile = (getProjectiles())[projectileId];
			return true;
		}
		return false;
	}

	void WorldModel::deleteAllObjs()
	{
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
		Obstacles::Iterator it = obstacles->begin();
		Obstacles::Iterator end = obstacles->end();		
		for(; it != end; ++it)
		{
			Obstacle *obstacle = it->second;
			if (obstacle->overlapping(rectangle)) return obstacle;
		}
		return NULL;
	}

	void WorldModel::MovePlayerObj::operator ()(const PlayerObjs::Pair &playerObjPair)
	{
		float fbMoveDistance = PlayerObj::getForwardBackwardSpeed();
		float strafeMoveDistance = PlayerObj::getStrafeSpeed();

		// ----- produce a movevector from current actions and angle of playerObj ------

		PlayerObj *playerObj = playerObjPair.second;
		Angle moveAngle;
		if (moveAlignedToAngle) moveAngle = playerObj->angle;
		else moveAngle = PI_F/2.0f;
		Vec moveVec(0.0f, 0.0f);

		const StateCmds &stateCmds(playerObj->getUserCmd().stateCmds);
		if (stateCmds.getCurrentState(Cmds::FORWARD))
		{
			moveVec += Vec(cos(moveAngle.getFloat()) * fbMoveDistance, sin(moveAngle.getFloat()) * fbMoveDistance);
		}

		if (stateCmds.getCurrentState(Cmds::BACKWARD))
		{
			moveVec += Vec(cos((moveAngle + Angle::PI).getFloat()) * fbMoveDistance, sin((moveAngle + Angle::PI).getFloat()) * fbMoveDistance);
		}

		if (stateCmds.getCurrentState(Cmds::LEFT))
		{
			moveVec += Vec(cos((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance, sin((moveAngle + Angle::PI/2.0f).getFloat()) * strafeMoveDistance);
		}

		if (stateCmds.getCurrentState(Cmds::RIGHT))
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

	bool WorldModel::performProjectileHit(GameObjId projectileId, ProjectileHit &projectileHit)
	{
		// get projectile
		
		Projectile *projectile = (getProjectiles())[projectileId];

		assert(projectile);
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
				Rectangle rectangle;
				targetPlayerObj->getRectangle(rectangle);
				float localMinHitDist = projectileLine.minCrossPoint(rectangle);
				if (Line::crossing(localMinHitDist) && (localMinHitDist < minHitDist))
				{
					hitPlayerObj = true;
					minHitDist = localMinHitDist;
					playerIdHit = targetPlayerObjId;
				}
			}
		}
		

		
		if (minHitDist <= 1.0f) // did hit any object?
		{
			Pos hitPos(projectileLine.getPosAlong(minHitDist));

			// Calculate and apply damage to playerobjects
			playerObjIt = getPlayerObjs().begin();
			playerObjEnd = getPlayerObjs().end();		
			for(; playerObjIt != playerObjEnd; ++playerObjIt)
			{
				GameObjId targetPlayerObjId = playerObjIt->first;
				PlayerObj *targetPlayerObj = playerObjIt->second;

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
				
				// apply damage to playerobject
				if (damage > 0)
				{
					std::cout << "damage =  " << damage << std::endl;
					targetPlayerObj->hurt(damage, projectile->getShooterId());
				}	
			}
			
			ProjectileHit projectileHitTmp(projectileId, hitPos);
			projectileHit = projectileHitTmp;

			return true;
		}

		return false;
	}

	bool WorldModel::isConsistent()
	{
		return getObstacles().isConsistent() &&
				getPlayerObjs().isConsistent() &&
				getProjectiles().isConsistent();
	}
};
