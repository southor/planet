#include "clientworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		Delete del;
		std::for_each(obstacles.begin(), obstacles.end(), del);
		std::for_each(playerObjs.begin(), playerObjs.end(), del);
		std::for_each(projectiles.begin(), projectiles.end(), del);
	}
};