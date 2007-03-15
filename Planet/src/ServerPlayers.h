#ifndef __ServerPlayers_h__
#define __ServerPlayers_h__

#include "ServerPlayer.h"

namespace Planet
{
	typedef IdMap<PlayerId, ServerPlayer*> ServerPlayers;

	template <typename MessageData>
	void pushMessageToAll(ServerPlayers &serverPlayers, const MessageData &messageData, int currentTime, int currentTick)
	{
		ServerPlayers::Iterator it = serverPlayers.begin();
		ServerPlayers::Iterator end = serverPlayers.end();
		for(; it != end; ++it)
		{
			it->second->link.pushMessage(messageData, currentTime, currentTick);
		}
	}

	void transmitAll(ServerPlayers &serverPlayers);
};

#endif
