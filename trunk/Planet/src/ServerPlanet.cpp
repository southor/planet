#include "ServerPlanet.h"

namespace Planet
{
	void ServerPlanet::performProjectileHits(ServerPlayers &players)
	{
		// TODO: add collision code
	}

	void ServerPlanet::handlePlayerShooting(PlayerId playerId, ServerPlayers &players)
	{	
		int currentTick = getTimeHandler()->getTick();
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		const UserCmd &userCmd(playerObj->getUserCmd());
		
		assert((userCmd.firstShotTick >= playerObj->getNextShootTick()) &&
				(playerObj->getNextShootTick() >= currentTickf));
		
		// Do any shooting
		int nShots = userCmd.nShots;
		Projectile::Type weapon = userCmd.weapon;
		for(int i=0; i<nShots; ++i)
		{
			
			//Tickf shootTick = playerObj->getShotTick(currentTick, i);
			//GameObjId projectileId = playerShoot(playerId, weapon, shootTick, userCmd.objLag);
			//Projectile *projectile = (getProjectiles())[projectileId];
			
			//GameObjId projectileId = getIdGenerator()->generateGameObjId();
			GameObjId projectileId = userCmd.firstProjectileId + i;
			if (playerTryShoot(playerId, currentTick, i, projectileId))
			{		
				Projectile *projectile = (getProjectiles())[projectileId];
				std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
				
				// send projectile to all clients
				AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
											projectile->getExt(), projectile->getShooterId(),
											projectile->getShootTick(), projectile->getObjLag());
				pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				//ServerPlayers::Iterator it = players.begin();
				//ServerPlayers::Iterator end = players.end();
				//for(; it != end; ++it)
				//{
				//	if (it->first != playerId) it->second->link.pushMessage(addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
				//}
			}
			else
			{
				RemoveProjectile removeProjectile(projectileId);
				players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			}
		}		
	}
};