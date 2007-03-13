#include "messages.h"
//#include "StandardInterExtraPolate.h"

namespace Planet
{
	//const UserCmd UserCmd::DEFAULT_USER_CMD = UserCmd(0, 0.0f, Projectile::DEFAULT_TYPE, 0, false, 0);
	//
	//void UserCmd::interExtraPolate(int tick1, const UserCmd& data1, int tick2, const UserCmd& data2, Tickf resultTick, UserCmd& resultData)
	//{
	//	// set by assumtion
	//	if (resultTick >= tick2) data2.assumeNext(resultData);
	//	else data1.assumeNext(resultData);

	//	// calculate aimAngle, assume continues rotation
	//	standardInterExtraPolate(tick1, data1.aimAngle, tick2, data2.aimAngle, resultTick, resultData.aimAngle);

	//	// set first shoot tick data
	//	resultData.firstShotTick = resultTick;
	//}

	//void UserCmd::clear()
	//{
	//	//stateCmds = 0;
	//	//aimAngle = 0.0f;
	//	//weapon = Projectile::DEFAULT_PROJECTILE;
	//	//nShots = 0;
	//	//shootAction = NOT_SHOOTING;
	//	*this = DEFAULT_USER_CMD;
	//	//firstShootTick = currentTick;
	//}

	//void UserCmd::assumeNext(UserCmd &resultData) const
	//{
	//	// set basics
	//	resultData = *this;

	//	// modify shooting		
	//	resultData.nShots = 0;
	//	resultData.firstShotTick += 1.0;
	//	//if (isShooting())
	//	//{
	//	//	resultData.shootAction = CONTINUE_SHOOTING;
	//	//}
	//	
	//	//if (shootAction == NOT_SHOOTING)
	//	//{
	//	//	resultData.nShots = 0;
	//	//}
	//	//else
	//	//{
	//	//	assert(isShooting());
	//	//	resultData.shootAction = CONTINUE_SHOOTING;
	//	//}
	//}

	//bool UserCmd::isConsistent(int currentTick) const
	//{
	//	if ((weapon < 0) || (weapon >= Projectile::N_TYPES)) return false;
	//	if (nShots < 0) return false;
	//	return firstShotTick >= static_cast<Tickf>(currentTick);
	//}
};
