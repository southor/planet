#include "worldmodel.h"
#include <algorithm>

namespace Prototype
{
	const Vec WorldModel::WORLD_SIZE = Vec(800.0f / 2.0f, 600.0f * (3.0f / 4.0f));

	WorldModel::~WorldModel()
	{
		Delete del;
		std::for_each(obstacles.begin(), obstacles.end(), del);
		std::for_each(playerObjs.begin(), playerObjs.end(), del);
		std::for_each(projectiles.begin(), projectiles.end(), del);
	}
};