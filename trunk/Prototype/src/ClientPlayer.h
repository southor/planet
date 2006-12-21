#ifndef __clientplayer_h__
#define __clientplayer_h__

#include "Player.h"
#include "ClientIdMap.h"

namespace Prototype
{
	typedef Player ClientPlayer;

	typedef ClientIdMap<size_t, ClientPlayer> ClientPlayers;
};

#endif