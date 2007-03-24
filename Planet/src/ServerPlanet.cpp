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
		PlayerObj *playerObj = new PlayerObj(color, playerPos, aimPos, SERVER_N_HISTORY_TICKS, getTimeHandler()->getTick(), &planetBody);
		
		printf("adding player object with playerId: %d (tick: %d, nextShootTick: %d)\n", playerId, getTimeHandler()->getTick(), playerObj->getNextShootTick());

		playerObjs.add(playerId, playerObj);
	}

	void ServerPlanet::handlePlayerShooting(PlayerId playerId, ServerPlayers &players)
	{	
		int currentTick = getTimeHandler()->getTick();
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		const UserCmd &userCmd(playerObj->getUserCmd());
		ClientIdGenerator *clientIdGenerator = players[playerId]->getIdGenerator();
		
		assert(userCmd.firstShotTick >= currentTickf);
		assert(playerObj->getNextShootTick() >= currentTickf);

		// remove projectiles if client created projectiles which we did't notice before
		while (userCmd.firstProjectileId > clientIdGenerator->getNextId())
		{
			RemoveProjectile removeProjectile(clientIdGenerator->generateGameObjId());
			players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
		}
		assert(userCmd.firstProjectileId == clientIdGenerator->getNextId());
		
		int nShots = userCmd.nShots;
		if (userCmd.nShots > 0)
		{
			assert(userCmd.firstShotTick >= playerObj->getNextShootTick());
		
			// Do any shooting		
			Projectile::Type weapon = userCmd.weapon;
			for(int i=0; i<nShots; ++i)
			{
				
				//Tickf shootTick = playerObj->getShotTick(currentTick, i);
				//GameObjId projectileId = playerShoot(playerId, weapon, shootTick, userCmd.objLag);
				//Projectile *projectile = (getProjectiles())[projectileId];
				
				//GameObjId projectileId = getIdGenerator()->generateGameObjId();
				//GameObjId projectileId = userCmd.firstProjectileId + i;
				GameObjId projectileId = clientIdGenerator->generateGameObjId();
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
	}
};