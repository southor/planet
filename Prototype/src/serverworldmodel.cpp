#include "serverworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ServerWorldModel::~ServerWorldModel()			
	{
		Delete del;
		std::for_each(obstacles.begin(), obstacles.end(), del);
		std::for_each(playerObjs.begin(), playerObjs.end(), del);
		std::for_each(projectiles.begin(), projectiles.end(), del);
	}
};