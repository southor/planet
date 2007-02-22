#include "WorldModel.h"

namespace Prototype
{
	const Vec WorldModel::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	void WorldModel::deleteAllObjs()
	{
		//Delete del;
		DeleteObstacle deleteObstacle;
		DeletePlayerObj deletePlayerObj;
		DeleteProjectile deleteProjectile;
		ForEach(getObstacles().begin(), getObstacles().end(), deleteObstacle);
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), deletePlayerObj);
		ForEach(getProjectiles().begin(), getProjectiles().end(), deleteProjectile);
	}

	bool WorldModel::isConsistent()
	{
		return getObstacles().isConsistent() &&
				getPlayerObjs().isConsistent() &&
				getProjectiles().isConsistent();
	}
};
