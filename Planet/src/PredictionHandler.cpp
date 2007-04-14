#include "PredictionHandler.h"
#include <iostream>

namespace Planet
{

	PredictionHandler::PredictionHandler(PlayerId playerId, ClientPlanet *planet) :
		userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS, UserCmd::interExtraPolate),
		planet(planet), latestServerInputTick(0), playerId(playerId), rePredictNeeded(false)
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
		assert(planet);

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
		return isTick0UserCmdConsistent();
	}
		

};

