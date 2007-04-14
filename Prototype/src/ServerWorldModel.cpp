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

	void ServerWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos, int fullHealth)
	{
		PlayerObj *playerObj = new PlayerObj(playerId, playerPos, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick(), fullHealth);
		playerObjs.add(playerId, playerObj);
	}

	GameObjId ServerWorldModel::addObstacle(const Rectangle &obstacleArea)
	{
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
			projectileIt->second->move();
		}
	}

	void ServerWorldModel::performProjectileHits(ServerPlayers &players, WorldRenderer *worldRenderer)
	{
		int currentTick = getTimeHandler()->getTick();
		
		std::vector<ProjectileHit> projectilesHit;

		Projectiles::Iterator projectileIt(getProjectiles().begin());
		Projectiles::Iterator projectileEnd(getProjectiles().end());
		for(; projectileIt != projectileEnd; ++projectileIt)
		{			
			Projectile *projectile = projectileIt->second;
			int objLag = projectile->getObjLag();

			// update the opponents and shooter to the correct tick
			if (objLag != 0)
			{
				updatePlayerObjsToTickData(currentTick - objLag);
				(getPlayerObjs())[projectile->getShooterId()]->updateToTickData(currentTick);
			}

			// test projectile hit
			ProjectileHit projectileHit(projectileIt->first, Pos(0.0f, 0.0f));			
			if (performProjectileHit(projectileIt->first, projectileHit))
			{
				// push projectile hit for later
				projectilesHit.push_back(projectileHit);

				// render explosion at server
				assert(worldRenderer);
				worldRenderer->projectileHit(projectile, projectileHit.hitPosition);
			}			
		}

		// update playerObjs back to current tick
		updatePlayerObjsToTickData(currentTick);

		// perform killing (playerObjs have already been hurt)
		PlayerObjs::Iterator playerObjsIt(getPlayerObjs().begin());
		PlayerObjs::Iterator playerObjsEnd(getPlayerObjs().end());
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{
			PlayerObj *playerObj = playerObjsIt->second;
			if (playerObj->isDead())
			{
				std::cout << "!!! a player was killed !!!  " << std::endl;

				// player was kiled
				PlayerId killerId = playerObj->lastHurter;
				
				// produce an unpredictable respawn place
				Pos tmpPos = playerObj->pos;
				size_t respawnPosId = static_cast<size_t>(abs(playerObj->health + static_cast<int>(tmpPos.x + tmpPos.y)) + getTimeHandler()->getTick()) % respawnPoss.size();
				
				Pos &respawnPos = respawnPoss[respawnPosId];						
				playerObj->respawn(respawnPos);

				// send kill message to all players
				Kill kill(killerId, playerObjsIt->first, respawnPos);
				pushMessageToAll(players, kill, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}
		}

		// send hits and remove projectiles
		if (projectilesHit.size() > 0)
		{
			std::vector<ProjectileHit>::iterator it = projectilesHit.begin();
			std::vector<ProjectileHit>::iterator end = projectilesHit.end();
			for(; it != end ; ++it)
			{
				bool result = projectiles.remove(it->projectileId);
				assert(result);
				
				// send message			
				pushMessageToAll(players, *it, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}
		}
	}

	void ServerWorldModel::handlePlayerShooting(PlayerId playerId, ServerPlayers &players)
	{	
		int currentTick = getTimeHandler()->getTick();
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		
		// Copy userCmd, we must have a seperate copy to alter while original is intact.
		UserCmd userCmd(playerObj->getUserCmd());

		ClientIdGenerator *clientIdGenerator = players[playerId]->getIdGenerator();
		int clientNShots = userCmd.nShots;
		
		assert(userCmd.firstShotTick >= currentTickf);
		assert(playerObj->getNextShootTick() >= currentTickf);

		// remove projectiles if client created projectiles which we did't notice before
		while (userCmd.firstProjectileId > clientIdGenerator->getNextId())
		{
			RemoveProjectile removeProjectile(clientIdGenerator->generateGameObjId());
			players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		}
		assert(userCmd.firstProjectileId == clientIdGenerator->getNextId());
		
		if (clientNShots > 0)
		{
			assert(userCmd.firstShotTick >= playerObj->getNextShootTick());
		
			// Do any shooting
			int serverNShots = 0;
			Tickf shotTick;
			Projectile::Type weapon = userCmd.weapon;
			for(int i=0; i<clientNShots; ++i)
			{
				GameObjId projectileId = clientIdGenerator->generateGameObjId();
				if (playerTryShoot(playerId, currentTick, i, projectileId, shotTick))
				{		
					if (serverNShots == 0) userCmd.firstShotTick = shotTick;
					
					Projectile *projectile = (getProjectiles())[projectileId];
					std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
					
					// send projectile to all clients
					AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
												projectile->getAngle().getFloat(), projectile->getShooterId(),
												projectile->getShootTick(), projectile->getObjLag());
					pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());

					++serverNShots;
				}
				else
				{					
					RemoveProjectile removeProjectile(projectileId);
					players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				}
			}

			userCmd.nShots = serverNShots;
			
			// Safe to overwrite PlayerObj.userCmd now, write changes
			playerObj->setUserCmd(&userCmd);
		}

	}
};

