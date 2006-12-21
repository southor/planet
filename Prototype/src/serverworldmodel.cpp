#include "serverworldmodel.h"
#include "basic.h"
#include <cmath>

namespace Prototype
{
	ServerWorldModel::~ServerWorldModel()			
	{
		deleteAllObjs();
	}

	size_t ServerWorldModel::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		return playerObjs.add(new PlayerObj(playerId, playerPos));
	}

	void ServerWorldModel::updatePlayerObjMovements()
	{
		Move move;
		ForEach(playerObjs.begin(), playerObjs.end(), move);
	}

	void ServerWorldModel::Move::operator ()(const PlayerObjContainer::Pair &playerObjPair)
	{		
		PlayerObj *playerObj = playerObjPair.second;
		float angle = playerObj->angle;
		if (playerObj->movingForward == true)
		{
			playerObj->pos += Vec(sin(angle) * 5.0f, cos(angle) * 5.0f);			
		}
		if (playerObj->movingBackward == true)
		{
			playerObj->pos += Vec(sin(angle + PI) * 5.0f, cos(angle + PI) * 5.0f);
		}
		if (playerObj->strafingLeft == true)
		{
			playerObj->pos += Vec(sin(angle - PI/2.0f) * 3.0f, cos(angle - PI/2.0f) * 3.0f);
		}
		if (playerObj->strafingRight == true)
		{
			playerObj->pos += Vec(sin(angle + PI/2.0f) * 3.0f, cos(angle + PI/2.0f) * 3.0f);
		}
	}
};