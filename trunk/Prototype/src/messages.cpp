#include "messages.h"
#include "StandardInterExtraPolate.h"
#include "IdGenerator.h"

namespace Prototype
{
	const UserCmd UserCmd::DEFAULT_USER_CMD = UserCmd(0, 0.0f, Projectile::DEFAULT_TYPE, 0, false, 0.0, GameObjId(0, 1), 0);
	
	void UserCmd::interExtraPolate(int tick1, const UserCmd& data1, int tick2, const UserCmd& data2, Tickf resultTick, UserCmd& resultData)
	{
		assert(data1.isConsistent());
		assert(data2.isConsistent());
		
		// set by assumtion
		if (resultTick >= tick2) data2.assumeNext(resultData);
		else data1.assumeNext(resultData);

		// calculate aimAngle, assume continues rotation
		standardInterExtraPolate(tick1, data1.aimAngle, tick2, data2.aimAngle, resultTick, resultData.aimAngle);
		
		// calculate objLag
		//standardInterExtraPolate(tick1, data1.objLag, tick2, data2.objLag, resultTick, resultData.objLag);

		// set first shoot tick data
		resultData.firstShotTick = resultTick;

		assert(resultData.isConsistent());
	}

	void UserCmd::clear(PlayerId playerId, int tick)
	{
		*this = getDefaultUserCmd(playerId);
		firstShotTick = static_cast<Tickf>(tick);
	}

	void UserCmd::assumeNext(UserCmd &resultData) const
	{
		// set basics
		resultData = *this;

		// modify shooting		
		resultData.nShots = 0;
		resultData.firstShotTick += 1.0;
		resultData.firstProjectileId += this->nShots;
	}

	UserCmd UserCmd::getDefaultUserCmd(PlayerId playerId)
	{
		UserCmd defaultUserCmd(DEFAULT_USER_CMD);
		defaultUserCmd.firstProjectileId = IdGenerator::getFirstGameObjId(playerId);
		return defaultUserCmd;
	}

	bool UserCmd::isConsistent(PlayerId playerId, int currentTick) const
	{		
		if (!isConsistent(currentTick)) return false;
		if (firstProjectileId.getCreatorId() != playerId) return false;
		return true;	
	}

	bool UserCmd::isConsistent(int currentTick) const
	{
		if (!isConsistent()) return false;
		return firstShotTick >= static_cast<Tickf>(currentTick);
	}

	bool UserCmd::isConsistent() const
	{
		if ((weapon < 0) || (weapon >= Projectile::N_TYPES)) return false;
		if (nShots < 0) return false;
		if (objLag < 0) return false;
		if (!firstProjectileId.isConsistent()) return false;		
		if (firstProjectileId.isPlayerObjId()) return false;
		return firstShotTick >= 0.0;
	}
};
