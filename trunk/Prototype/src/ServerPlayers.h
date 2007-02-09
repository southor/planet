#ifndef __serverplayers_h__
#define __serverplayers_h__

#include "ServerPlayer.h"

namespace Prototype
{

	typedef IdMap<size_t, ServerPlayer> ServerPlayers;

	template <typename MessageData>
	void pushMessageToAll(ServerPlayers &serverPlayers, const MessageData &messageData)
	{
		ServerPlayers::Iterator it = serverPlayers.begin();
		ServerPlayers::Iterator end = serverPlayers.end();
		for(; it != end; ++it)
		{
			it->second.link.pushMessage(messageData);
		}
	}

	void transmitAll(ServerPlayers &serverPlayers);

};

#endif
