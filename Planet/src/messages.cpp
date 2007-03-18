#include "messages.h"
//#include "StandardInterExtraPolate.h"

namespace Planet
{
	
	const UserCmd UserCmd::DEFAULT_USER_CMD = UserCmd(0, Pos(0.0f, 0.0f, 0.0f),
														Projectile::DEFAULT_TYPE, 0, false, 0, GameObjId(), 0);
	
	void UserCmd::interExtraPolate(int tick1, const UserCmd& data1, int tick2, const UserCmd& data2, Tickf resultTick, UserCmd& resultData)
	{
		// set by assumtion
		if (resultTick >= tick2) data2.assumeNext(resultData);
		else data1.assumeNext(resultData);

		//// calculate aimAngle, assume continues rotation
		standardInterExtraPolate(tick1, data1.aimPos, tick2, data2.aimPos, resultTick, resultData.aimPos);
		
		// calculate objLag
		standardInterExtraPolate(tick1, data1.objLag, tick2, data2.objLag, resultTick, resultData.objLag);

		// set first shoot tick data
		resultData.firstShotTick = resultTick;
	}

	void UserCmd::clear()
	{
		*this = DEFAULT_USER_CMD;
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

	bool UserCmd::isConsistent(int currentTick) const
	{
		if ((weapon < 0) || (weapon >= Projectile::N_TYPES)) return false;
		if (nShots < 0) return false;
		return firstShotTick >= static_cast<Tickf>(currentTick);
	}
};
