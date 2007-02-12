#ifndef __serverplayer_h__
#define __serverplayer_h__

#include "Player.h"
#include "Link.h"
#include "IdMap.h"

namespace Prototype
{
	class ServerPlayer : public Player
	{
	public:
		Link link;
		int latestTick;

		ServerPlayer() : latestTick(0)	{}
		
		ServerPlayer(const Color &color, MessageSender *messageSender, MessageReciever *messageReciever)
			: Player(color), link(messageSender, messageReciever), latestTick(0)
		{}
	};
};

#endif
