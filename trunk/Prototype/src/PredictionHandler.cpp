#include "PredictionHandler.h"
#include <iostream>

namespace Prototype
{

	PredictionHandler::PredictionHandler(PlayerId playerId, ClientWorldModel *worldModel) :
		userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS, UserCmd::interExtraPolate),
		worldModel(worldModel), latestServerInputTick(0), playerId(playerId), rePredictNeeded(false)
	{		
		userCmdHistoryList.setDefaultData(UserCmd::getDefaultUserCmd(playerId));
		assert(isTick0UserCmdConsistent());
	}

	int PredictionHandler::getlastTick()
	{
		return getPlayerObj(playerId)->getLastStoredTick();
	}

	void PredictionHandler::predict(int fromTick, int toTick)
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
			
			worldModel->updatePlayerObjMovement(playerId);
			playerObj->updateNextShootTick(tick);
			
			playerObj->storeToTickData(tick + 1);
		}
	}

	// debug
	bool PredictionHandler::isTick0UserCmdConsistent()
	{
		//if (userCmdHistoryList.nextTick > userCmdHistoryList.size)
		//{
		//	for (int i = 0; i < userCmdHistoryList.size; i++)
		//	{
		//		int firstTick = userCmdHistoryList.firstTick();
		//		printf("i: %d, weapon: %d\n", i, userCmdHistoryList.data[i].weapon);
		//		assert(userCmdHistoryList.data[i].isConsistent());
		//	}
		//}
		UserCmd testUserCmd;
		userCmdHistoryList.getData(0, testUserCmd);
		bool consistent = testUserCmd.isConsistent(playerId, 0);
		//printf("isTick0UserCmdConsistent() weapon: %d\n", testUserCmd.weapon);
		assert(consistent);
		return consistent;
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

