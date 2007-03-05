#include "PredictionHandler.h"
#include <iostream>

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
		assert(worldModel);
		//assert(getlastTick(playerId) >= fromTick);
		
		//if (fromTick != 0) std::cout << "a prediction from " << fromTick << " to " << toTick << std::endl;
			
		playerObj->updateToTickData(fromTick);
		for(int tick = fromTick; tick < toTick; ++tick)
		{
			UserCmd userCmd;
			userCmdHistoryList.getData(tick, userCmd);			
			playerObj->setUserCmd(&userCmd);
			
			
			worldModel->updatePlayerObjMovement(playerId, static_cast<float>(TimeHandler::TICK_DELTA_TIME));
			
			playerObj->storeToTickData(tick + 1);
		}
	}
		

};

