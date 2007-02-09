#include "WorldModel.h"

namespace Prototype
{
	const Vec WorldModel::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	void WorldModel::deleteAllObjs()
	{
		Delete del;
		ForEach(getObstacles().begin(), getObstacles().end(), del);
		ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), del);
		ForEach(getProjectiles().begin(), getProjectiles().end(), del);
	}

	bool WorldModel::isConsistent()
	{
		return getObstacles().isConsistent() &&
				getPlayerObjs().isConsistent() &&
				getProjectiles().isConsistent();
	}
};