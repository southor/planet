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
		
	public:

		PredictionHandler() : userCmdHistoryList(CLIENT_PREDICTION_N_HISTORY_TICKS), worldModel(NULL)
		{}
		
		void setWorldModel(ClientWorldModel *worldModel)			{ this->worldModel = worldModel; }

		void setUserCmd(const UserCmd &userCmd, int tick)			{ userCmdHistoryList.setData(tick, userCmd); }

		// Will overwrite previous predictions from fromTick.
		void predict(PlayerId playerId, int fromTick, int toTick);
		
		// Uses previous prediction to predict further
		inline void predict(PlayerId playerId, int toTick)
		{
			predict(playerId, getlastTick(playerId), toTick);
		}
	
	};
};

#endif
