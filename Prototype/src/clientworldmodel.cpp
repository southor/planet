#include "clientworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		deleteAllObjs();
	}

	void ClientWorldModel::addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos)
	{
		playerObjs.add(playerObjId, new PlayerObj(playerId, playerPos));
	}
};