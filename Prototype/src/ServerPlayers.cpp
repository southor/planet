#include "ServerPlayers.h"

namespace Prototype
{
	void transmitAll(ServerPlayers &serverPlayers)
	{
		ServerPlayers::Iterator it = serverPlayers.begin();
		ServerPlayers::Iterator end = serverPlayers.end();
		for(; it != end; ++it)
		{
			//std::cout << "50.1 ";
			it->second->link.transmit();
			//std::cout << "50.2 ";
		}
	}

};

