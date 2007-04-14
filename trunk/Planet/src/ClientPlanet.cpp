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
			GameObjId projectileId = getIdGenerator()->generateGameObjId();
			assert(projectileId == (userCmd.firstProjectileId + i));
			playerTryShoot(playerId, currentTick, i, projectileId);
		}		
	}
};