#ifndef __serverplayer_h__
#define __serverplayer_h__

#include "Player.h"
#include "Link.h"
#include "ServerIdMap.h"

namespace Prototype
{
	class ServerPlayer : public Player
	{
	public:
		Link link;
		
		ServerPlayer(const Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
			: Player(color), link(messageSender, messageReciever)
		{}
	};

	typedef ServerIdMap<size_t, ServerPlayer> ServerPlayers;
};

#endif