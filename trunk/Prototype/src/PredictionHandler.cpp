#include "PredictionHandler.h"
#include <iostream>

namespace Prototype
{

	PredictionHandler::PredictionHandler() :
		userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS, UserCmd::interExtraPolate),
		//userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS),
		worldModel(NULL), latestServerInputTick(0)
	{
		userCmdHistoryList.setDefaultData(UserCmd::DEFAULT_USER_CMD);
		assert(isTick0UserCmdConsistent());

	}

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

		//debug
		assert(isTick0UserCmdConsistent());

		playerObj->updateToTickData(fromTick);
		for(int tick = fromTick; tick < toTick; ++tick)
		{
			UserCmd userCmd;
			userCmdHistoryList.getData(tick, userCmd);
			playerObj->setUserCmd(&userCmd);
			//playerObj->setUserCmd = userCmd;
			
			worldModel->updatePlayerObjMovement(playerId);
			playerObj->updateNextShootTick(tick);
			
			playerObj->storeToTickData(tick + 1);
		}
	}

	// debug
	bool PredictionHandler::isTick0UserCmdConsistent()
	{
		UserCmd testUserCmd;
		userCmdHistoryList.getData(0, testUserCmd);
		return testUserCmd.isConsistent(0);
	}

	//debug
	bool PredictionHandler::isConsistent()
	{
		//just test some things for now

		if (!userCmdHistoryList.isConsistent()) return false;
		//std::cout << "userCmdList consistent" << std::endl;
		return isTick0UserCmdConsistent();
	}
		

};

