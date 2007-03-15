#ifndef __ServerPlayer_h__
#define __ServerPlayer_h__

#include "Player.h"
#include "Link.h"
#include "IdMap.h"
#include "HistoryList.h"
#include "TimeHandler.h"


namespace Planet
{
	class ServerPlayer : public Player
	{
	private:
		static const int USER_CMD_HISTORY_TIME = 200;
		static const int USER_CMD_HISTORY_SIZE = USER_CMD_HISTORY_TIME / TimeHandler::TICK_DELTA_TIME + 1;

		//% HistoryList<UserCmd> userCmdHistoryList;
	public:
		Link link;
		
		

		ServerPlayer() //% : userCmdHistoryList(USER_CMD_HISTORY_SIZE, UserCmd::interExtraPolate)
		{
			//% userCmdHistoryList.setDefaultData(UserCmd::DEFAULT_USER_CMD);
		}
		
		ServerPlayer(const Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
			: Player(color), link(messageSender, messageReciever) //% , userCmdHistoryList(USER_CMD_HISTORY_SIZE, UserCmd::interExtraPolate)
		{
			//% userCmdHistoryList.setDefaultData(UserCmd::DEFAULT_USER_CMD);
		}

		//% void getUserCmd(UserCmd &userCmd, int tick)					{ userCmdHistoryList.getData(tick, userCmd);
		//%															  assert(userCmd.isConsistent(tick)); }
		//% void setUserCmd(const UserCmd &userCmd, int tick)			{ assert(userCmd.isConsistent(tick));
		//%															  userCmdHistoryList.setData(tick, userCmd); }

	};
};

#endif
