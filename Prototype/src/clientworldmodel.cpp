#include "clientworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		deleteAllObjs();
	}

	void ClientWorldModel::addPlayer(size_t playerId, const Pos &playerPos)
	{
		playerObjs.add(playerId, new PlayerObj(playerId, playerPos));
	}
};