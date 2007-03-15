#include "ServerPlayers.h"

namespace Planet
{
	void transmitAll(ServerPlayers &serverPlayers)
	{
		ServerPlayers::Iterator it = serverPlayers.begin();
		ServerPlayers::Iterator end = serverPlayers.end();
		for(; it != end; ++it)
		{
			it->second->link.transmit();
		}
	}
};

