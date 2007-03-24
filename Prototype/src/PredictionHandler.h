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
		//int getlastTick(PlayerId playerId);
		int getlastTick();
		PlayerId playerId;
		bool rePredictNeeded;

		ClientWorldModel *worldModel;

		int latestServerInputTick;

		// Will overwrite previous predictions from fromTick.
		//void predict(PlayerId playerId, int fromTick, int toTick);
		void predict(int fromTick, int toTick);

		// Repredicts the old prediction, overwrites old data but do not produce new one.
		//inline void rePredict(PlayerId playerId, int fromTick)
		inline void rePredict(int fromTick)
		{
			//int latestTick = getlastTick(playerId);
			int latestTick = getlastTick();
			//predict(playerId, fromTick, latestTick);
			predict(fromTick, latestTick);
			rePredictNeeded = false;
		}
		
	public:

		PredictionHandler(PlayerId playerId, ClientWorldModel *worldModel);
		
		//void setWorldModel(ClientWorldModel *worldModel)			{ this->worldModel = worldModel; }

		void getUserCmd(UserCmd &userCmd, int tick)					{ userCmdHistoryList.getData(tick, userCmd);
																	  assert(userCmd.isConsistent(tick)); }
		void setUserCmd(const UserCmd &userCmd, int tick)			{ assert(userCmd.isConsistent(tick));
																	  userCmdHistoryList.setData(tick, userCmd); }


		
		// Uses previous prediction to predict further
		//inline void predict(PlayerId playerId, int toTick)
		inline void predict(int toTick)
		{			
			rePredictIfNeeded();
			assert(!rePredictNeeded);
			//int latestTick = getlastTick(playerId);
			int latestTick = getlastTick();
			//predict(playerId, latestTick, toTick);
			predict(latestTick, toTick);
		}

		//void serverInput(PlayerId playerId, int inputTick)
		void serverInput(int inputTick)
		{
			if (latestServerInputTick < inputTick)
			{
				//rePredict(playerId, inputTick);
				//rePredict(inputTick);
				rePredictNeeded = true;
				latestServerInputTick = inputTick;
			}
		}

		void rePredictIfNeeded()
		{
			if (rePredictNeeded) rePredict(latestServerInputTick);
		}

		// debug
		bool isTick0UserCmdConsistent();
		bool isConsistent();
	
	};
};

#endif
