#include "PredictionHandler.h"

namespace Prototype
{

	int PredictionHandler::getlastTick(PlayerId playerId)
	{
		return getPlayerObj(playerId)->getLastStoredTick();
	}

	void PredictionHandler::predict(PlayerId playerId, int fromTick, int toTick)
	{
		PlayerObj *playerObj = getPlayerObj(playerId);
		
		assert(playerObj);
		//assert(getlastTick(playerId) >= fromTick);
		assert(worldModel);
		
		playerObj->updateToTickData(fromTick);
		for(int tick = fromTick + 1; tick <= toTick; ++tick)
		{
			UserCmd userCmd;
			userCmdHistoryList.getData(tick, userCmd);
			playerObj->setUserCmd(&userCmd);
			worldModel->updatePlayerObjMovement(playerId, static_cast<float>(TimeHandler::TICK_DELTA_TIME));
			playerObj->storeToTickData(tick);
		}
	}
		

};

