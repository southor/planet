#ifndef __clientplayer_h__
#define __clientplayer_h__

#include "Player.h"
#include "IdMap.h"

namespace Prototype
{
	typedef Player ClientPlayer;

	typedef IdMap<size_t, ClientPlayer> ClientPlayers;
};

#endif
