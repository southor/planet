#ifndef __predictionhandler_h__
#define __predictionhandler_h__

//#include "PlayerObj.h"
#include "HistoryList.h"
#include "PlayerObj.h"
#include "ClientWorldModel.h"
#include "ClientTimeHandler.h"

namespace Prototype
{
	class PredictionHandler
	{
	private:
		HistoryList<UserCmd> userCmdHistoryList;
		
		PlayerObj* getPlayerObj(PlayerId playerId)					{ return (worldModel->getPlayerObjs())[playerId]; }
		int getlastTick(PlayerId playerId);

		ClientWorldModel *worldModel;

		int latestServerInputTick;

		// Will overwrite previous predictions from fromTick.
		void predict(PlayerId playerId, int fromTick, int toTick);
		
	public:

		PredictionHandler();
		
		void setWorldModel(ClientWorldModel *worldModel)			{ this->worldModel = worldModel; }

		void getUserCmd(UserCmd &userCmd, int tick)					{ userCmdHistoryList.getData(tick, userCmd);
																	  assert(userCmd.isConsistent()); }
		void setUserCmd(const UserCmd &userCmd, int tick)			{ assert(userCmd.isConsistent());
																	  userCmdHistoryList.setData(tick, userCmd); }

		// Repredicts the old prediction, overwrites old data but do not produce new one.
		inline void rePredict(PlayerId playerId, int fromTick)
		{
			int latestTick = getlastTick(playerId);
			predict(playerId, fromTick, latestTick);
		}
		
		// Uses previous prediction to predict further
		inline void predict(PlayerId playerId, int toTick)
		{			
			int latestTick = getlastTick(playerId);
			predict(playerId, latestTick, toTick);
		}

		void serverInput(PlayerId playerId, int inputTick)
		{
			if (latestServerInputTick < inputTick)
			{
				rePredict(playerId, inputTick);
				latestServerInputTick = inputTick;
			}
		}

		// debug
		bool isTick0UserCmdConsistent();
		bool isConsistent();
	
	};
};

#endif
