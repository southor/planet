#include "ServerPlanet.h"
#include "Color.h"

namespace Planet
{
	void ServerPlanet::performProjectileHits(ServerPlayers &players)
	{
		// TODO: add collision code
	}

	void ServerPlanet::addPlayerObj(PlayerId playerId, const Pos &playerPos)
	{
		Color color = Color::RED; 
		Pos aimPos(playerPos + playerPos.getOrtoganal());
		PlayerObj *playerObj = new PlayerObj(playerId, color, playerPos, aimPos, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick(), &planetBody);
		
		printf("adding player object with playerId: %d (tick: %d, nextShootTick: %d)\n", playerId, getTimeHandler()->getTick(), playerObj->getNextShootTick());

		playerObjs.add(playerId, playerObj);
	}

	void ServerPlanet::handlePlayerShooting(PlayerId playerId, ServerPlayers &players)
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
												projectile->getExt(), projectile->getShooterId(),
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