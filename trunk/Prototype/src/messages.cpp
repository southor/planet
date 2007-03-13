#include "messages.h"
#include "StandardInterExtraPolate.h"

namespace Prototype
{
	
	const UserCmd UserCmd::DEFAULT_USER_CMD = UserCmd(0, 0.0f, Projectile::DEFAULT_TYPE, 0, UserCmd::NOT_SHOOTING);
	
	void UserCmd::interExtraPolate(int tick1, const UserCmd& data1, int tick2, const UserCmd& data2, Tickf resultTick, UserCmd& resultData)
	{
		// set by assumtion
		if (resultTick >= tick2) data2.assumeNext(resultData);
		else data1.assumeNext(resultData);

		// calculate aimAngle, assume continues rotation
		standardInterExtraPolate(tick1, data1.aimAngle, tick2, data2.aimAngle, resultTick, resultData.aimAngle);
	}

	void UserCmd::clear()
	{
		//stateCmds = 0;
		//aimAngle = 0.0f;
		//weapon = Projectile::DEFAULT_PROJECTILE;
		//nShots = 0;
		//shootAction = NOT_SHOOTING;
		*this = DEFAULT_USER_CMD;
	}

	void UserCmd::assumeNext(UserCmd &resultData) const
	{
		// set basics
		resultData = *this;

		// modify shooting		
		if (shootAction != NOT_SHOOTING)
		{
			resultData.shootAction = CONTINUE_SHOOTING;
		}
		resultData.nShots = 0;
		//if (shootAction == NOT_SHOOTING)
		//{
		//	resultData.nShots = 0;
		//}
		//else
		//{
		//	assert(isShooting());
		//	resultData.shootAction = CONTINUE_SHOOTING;
		//}
	}

	bool UserCmd::isConsistent() const
	{
		if ((weapon < 0) || (weapon >= Projectile::N_TYPES)) return false;
		if (nShots < 0) return false;
		return ((shootAction == START_SHOOTING) || (shootAction == CONTINUE_SHOOTING) || (shootAction == NOT_SHOOTING));
	}
};
