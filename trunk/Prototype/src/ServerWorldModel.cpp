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
		PlayerObj *playerObj = new PlayerObj(playerPos, 1, getTimeHandler()->getTick());
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

	void ServerWorldModel::updateProjectileMovements(ServerPlayers &players)
	{
		MoveProjectile move(&obstacles, &players, *this, &(getPlayerObjs()), &respawnPoss);
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
			//Projectiles::Iterator it2 = projectiles.begin();
			//Projectiles::Iterator it2End = projectiles.end();
			//for(; it2 != it2End; ++it2)
			//{
			//	std::cout << "\tprojectile left: " << it2->first << std::endl;
			//}
		}
	}



	void ServerWorldModel::MoveProjectile::operator ()(const Projectiles::Pair &projectilePair)
	{
		assert(players); // must be able to send updates
		assert(obstacles);
		assert(playerObjs);
		
		// move projectile
		Projectile *projectile = projectilePair.second;
		Vec moveVec(projectile->getLine().getDirection() * projectile->getSpeed());
		projectile->setPos(projectile->getPos() + moveVec);
		Line projectileLine(projectile->getLine());


		// Hit collision, find hit point

		float minHitDist = 2.0f;			
		GameObjId obstacleIdHit;

		Obstacles::Iterator obstacleIt = obstacles->begin();
		Obstacles::Iterator obstacleEnd = obstacles->end();		
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

		PlayerObjs::Iterator playerObjIt = playerObjs->begin();
		PlayerObjs::Iterator playerObjEnd = playerObjs->end();		
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
			//std::cout << "((((( projectile hit! )))))" << std::endl;
			
		}
		
	}

	GameObjId ServerWorldModel::playerShoot(PlayerId playerId, Projectile::Type weapon, Tickf shootTick)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());
		Angle angle = playerObj->angle;

		//GameObjId projectileId = getProjectiles().findFreeId();
		GameObjId projectileId = getIdGenerator()->generateGameObjId();
		Projectile *projectile = new Projectile(weapon, pos, angle, playerId, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick(), shootTick);		
		getProjectiles().add(projectileId, projectile);

		return projectileId;
	}

	void ServerWorldModel::handlePlayerShooting(PlayerId playerId, std::vector<GameObjId> &shots)
	{	
		int currentTick = getTimeHandler()->getTick();
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		const UserCmd &userCmd(playerObj->getUserCmd());
		
		/**
		 * Fix problems with lost UserCmd messages, problems arises
		 * when a CONTINUE_SHOOTING was recieved with no prior START_SHOOTING.
		 */
		Tickf nextShootTick = playerObj->getNextShootTick();
		if ((userCmd.shootAction == UserCmd::CONTINUE_SHOOTING) && (nextShootTick < currentTickf))
		{
			assert(false); // should not happen with TCP
			
			Tickf shootInterval = Projectile::getShootInterval(userCmd.weapon);
			int nShotIntervalDelay = 1 + static_cast<int>((currentTickf - nextShootTick) / shootInterval);
			
			// Delay nextShootTick
			playerObj->setNextShootTick(nextShootTick + static_cast<Tickf>(nShotIntervalDelay) * shootInterval);

			assert(playerObj->getNextShootTick() >= currentTickf);
		}
		
		// Do any shooting
		int nShots = userCmd.nShots;
		Projectile::Type weapon = userCmd.weapon;
		for(int i=0; i<nShots; ++i)
		{
			std::cout << "server: handlePlayerShooting: ";
			if (userCmd.shootAction == UserCmd::START_SHOOTING) std::cout << "start shooting" << std::endl;
			else if (userCmd.shootAction == UserCmd::CONTINUE_SHOOTING) std::cout << "continue shooting" << std::endl;
			else std::cout << "not shooting!";
			std::cout << std::endl;
			
			Tickf shootTick = playerObj->getShotTick(currentTick, i);
			playerObj->updateNextShootTime(currentTick);
			shots.push_back(playerShoot(playerId, weapon, shootTick));
		}
	}
};

