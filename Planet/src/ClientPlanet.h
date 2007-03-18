#ifndef __ClientPlanet_h__
#define __ClientPlanet_h__

#include "Planet.h"
#include "ClientGlobalAccess.h"

namespace Planet
{
	class ClientPlanet : public Planet, public ClientGlobalAccess
	{
	public:
		ClientPlanet(ClientGlobalObj *clientGlobalObj) : ClientGlobalAccess(clientGlobalObj)
		{}
	};
};

#endif
