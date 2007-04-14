#ifndef __ServerPlayer_h__
#define __ServerPlayer_h__

#include "Player.h"
#include "Link.h"
#include "IdMap.h"
//#include "HistoryList.h"
#include "TimeHandler.h"
#include "IdGenerator.h"

namespace Planet
{
	class ServerPlayer : public Player
	{
	private:
		static const int USER_CMD_HISTORY_TIME = 200;
		static const int USER_CMD_HISTORY_SIZE = USER_CMD_HISTORY_TIME / TimeHandler::TICK_DELTA_TIME + 1;

		HistoryList<UserCmd> userCmdHistoryList;

		ClientIdGenerator clientIdGenerator;
	public:
		Link link;

		ServerPlayer(PlayerId playerId, const Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
			: Player(color), link(messageSender, messageReciever), userCmdHistoryList(USER_CMD_HISTORY_SIZE, UserCmd::interExtraPolate)
		{
			clientIdGenerator.setPlayerId(playerId);
			
			UserCmd defaultUserCmd(UserCmd::DEFAULT_USER_CMD);
			defaultUserCmd.firstProjectileId = ClientIdGenerator::getFirstGameObjId(playerId);			
			userCmdHistoryList.setDefaultData(defaultUserCmd);

			assert(clientIdGenerator.getNextId() == defaultUserCmd.firstProjectileId);
		}

		void getUserCmd(UserCmd &userCmd, int tick)					{ userCmdHistoryList.getData(tick, userCmd);
																	  assert(userCmd.isConsistent(tick)); }
		void setUserCmd(const UserCmd &userCmd, int tick)			{ assert(userCmd.isConsistent(tick));
																	  userCmdHistoryList.setData(tick, userCmd); }

		inline ClientIdGenerator* getIdGenerator()					{ return &clientIdGenerator; }
	};
};

#endif
