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

		ServerPlayer()		{}
		
		ServerPlayer(const Color &color, Link link)
			: Player(color), link(link)
		{}
	};
};

#endif