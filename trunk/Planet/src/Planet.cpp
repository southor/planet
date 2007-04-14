#include "Planet.h"
#include "Cmds.h"

namespace Planet
{

	Planet::Planet()
	{}

	void Planet::updatePlayerObjsToTickData(Tickf tick)
	{
		PlayerObjs::Iterator it = getPlayerObjs().begin();
		PlayerObjs::Iterator end = getPlayerObjs().end();
		for(; it != end; ++it)
		{
			it->second->updateToTickData(tick);
		}
	}

	void Planet::playerShoot(PlayerId playerId, Projectile::Type weapon, Tickf shootTick, int objLag, GameObjId projectileId)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];
		Pos pos(playerObj->getPos());


		Projectile *projectile = new Projectile(weapon, pos, Vec(1.0f, 1.0f, 1.0f), playerId, 1, shootTick, objLag);		
		getProjectiles().add(projectileId, projectile);
	}

	bool Planet::playerTryShoot(PlayerId playerId, int currentTick, int shotN, GameObjId projectileId, Tickf &shotTick)
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
	
	void Planet::updatePlayerObjMovements()
	{
		PlayerObjs::Iterator playerObjsIt = getPlayerObjs().begin();
		PlayerObjs::Iterator playerObjsEnd = getPlayerObjs().end();
		for(; playerObjsIt != playerObjsEnd; ++playerObjsIt)
		{				
			PlayerId playerId = playerObjsIt->first;
			
			updatePlayerObjMovement(playerId);
		}
	}

	void Planet::updatePlayerObjMovement(PlayerId playerId)
	{
		PlayerObj *playerObj = getPlayerObjs()[playerId];		
		
		UserCmd userCmd = playerObj->getUserCmd();
		
		assert(playerObj->isConsistent());
		
		playerObj->ship.logic(userCmd);
	}

	void Planet::deleteAllObjs()
	{
		DeleteSecond<PlayerObjs::Pair> deletePlayerObj;
		DeleteSecond<Projectiles::Pair> deleteProjectile;
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), deletePlayerObj);
		ForEach(getProjectiles().begin(), getProjectiles().end(), deleteProjectile);
	}

	bool Planet::isConsistent()
	{
		return getPlayerObjs().isConsistent() &&
				getProjectiles().isConsistent();
	}
};
