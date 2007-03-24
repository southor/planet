#include "PredictionHandler.h"
#include <iostream>

namespace Planet
{

	PredictionHandler::PredictionHandler(PlayerId playerId, ClientPlanet *planet) :
		userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS, UserCmd::interExtraPolate),
		//userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS),
		planet(planet), latestServerInputTick(0), playerId(playerId), rePredictNeeded(false)
	{		
		userCmdHistoryList.setDefaultData(UserCmd::getDefaultUserCmd(playerId));
		assert(isTick0UserCmdConsistent());
	}

	//int PredictionHandler::getlastTick(PlayerId playerId)
	int PredictionHandler::getlastTick()
	{
		return getPlayerObj(playerId)->getLastStoredTick();
	}

	//void PredictionHandler::predict(PlayerId playerId, int fromTick, int toTick)
	void PredictionHandler::predict(int fromTick, int toTick)
	{
		PlayerObj *playerObj = getPlayerObj(playerId);
		
		assert(playerObj);
		assert(planet);
		//assert(getlastTick(playerId) >= fromTick);
		
		//if (fromTick != 0) std::cout << "a prediction from " << fromTick << " to " << toTick << std::endl;

		//debug
		assert(isTick0UserCmdConsistent());

		playerObj->updateToTickData(fromTick);
		for (int tick = fromTick; tick < toTick; ++tick)
		{
			UserCmd userCmd;
			userCmdHistoryList.getData(tick, userCmd);
			playerObj->setUserCmd(&userCmd);
			
			planet->updatePlayerObjMovement(playerId);
			playerObj->updateNextShootTick(tick);
			
			playerObj->storeToTickData(tick + 1);
		}
	}

	// debug
	bool PredictionHandler::isTick0UserCmdConsistent()
	{
		UserCmd testUserCmd;
		userCmdHistoryList.getData(0, testUserCmd);
		bool consistent = testUserCmd.isConsistent(playerId, 0);
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

