#include "ClientPlanet.h"

namespace Planet
{
	void ClientPlanet::addPlayerObj(PlayerId playerId, const Color &playerColor, const Pos &playerPos, const Pos &playerAimPos, bool isMe, int tick)
	{
		playerObjs.add(playerId, new PlayerObj(playerId, playerColor, playerPos, playerAimPos, (isMe ? CLIENT_PREDICTION_N_HISTORY_TICKS : CLIENT_INTERPOOLATION_N_HISTORY_TICKS), tick, &planetBody));
	}
	
	void ClientPlanet::addProjectile(GameObjId projectileId, AddProjectile *addProjectile)
	{
		Projectile *projectile = new Projectile(addProjectile->type, addProjectile->pos, addProjectile->ext, addProjectile->shooterId, CLIENT_INTERPOOLATION_N_HISTORY_TICKS, addProjectile->shootTick, addProjectile->objLag);
		assert(projectile);
		if (projectile) projectiles.add(projectileId, projectile);
	}

	void ClientPlanet::handlePlayerShooting(PlayerId playerId)
	{
		int currentTick = static_cast<int>(getTimeHandler()->getStepTick());
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
			GameObjId projectileId = getIdGenerator()->generateGameObjId();
			assert(projectileId == (userCmd.firstProjectileId + i));
			//if (
				playerTryShoot(playerId, currentTick, i, projectileId);
			//{		
			//	Projectile *projectile = (getProjectiles())[projectileId];
			//	std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
			//	
			//	// send projectile to all clients
			//	AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
			//								projectile->getAngle().getFloat(), projectile->getShooterId(),
			//								projectile->getShootTick(), projectile->getObjLag());
			//	pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//	//ServerPlayers::Iterator it = players.begin();
			//	//ServerPlayers::Iterator end = players.end();
			//	//for(; it != end; ++it)
			//	//{
			//	//	if (it->first != playerId) it->second->link.pushMessage(addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//	//}
			//}
			//else
			//{
			//	RemoveProjectile removeProjectile(projectileId);
			//	players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//}
		}		
	}
};