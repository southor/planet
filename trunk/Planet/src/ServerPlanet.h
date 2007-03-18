#ifndef __ServerPlanet_h__
#define __ServerPlanet_h__

#include "Planet.h"
#include "ServerGlobalAccess.h"
#include "ServerPlayers.h"

namespace Planet
{
	class ServerPlanet : public Planet, public ServerGlobalAccess
	{
	public:
		ServerPlanet(ServerGlobalObj *serverGlobalObj) : ServerGlobalAccess(serverGlobalObj) {}

		void performProjectileHits(ServerPlayers &players);
		void addPlayerObj(PlayerId playerId, const Pos &playerPos);

		void handlePlayerShooting(PlayerId playerId, ServerPlayers &players);
	};
};

#endif
