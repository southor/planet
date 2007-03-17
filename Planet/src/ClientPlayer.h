#ifndef __clientplayer_h__
#define __clientplayer_h__

#include "Player.h"
#include "IdMap.h"

namespace Planet
{
	typedef Player ClientPlayer;

	typedef IdMap<PlayerId, ClientPlayer*> ClientPlayers;
};

#endif
