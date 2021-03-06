#ifndef __ClientPlanet_h__
#define __ClientPlanet_h__

#include "Planet.h"
#include "ClientGlobalAccess.h"
#include "messages.h"

namespace Planet
{
	class ClientPlanet : public Planet, public ClientGlobalAccess
	{
	public:
		ClientPlanet(ClientGlobalObj *clientGlobalObj) : ClientGlobalAccess(clientGlobalObj)
		{}

		void init(const std::string &map)						{ planetBody.init(true, map); }

		void addPlayerObj(PlayerId playerId, const Color &playerColor, const Pos &playerPos, const Pos &playerAimPos, bool isMe, int tick);
		void addProjectile(GameObjId projectileId, AddProjectile *addProjectile);


		void handlePlayerShooting(PlayerId playerId);

	};
};

#endif
