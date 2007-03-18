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

		void addProjectile(GameObjId projectileId, AddProjectile *addProjectile);

	};
};

#endif
