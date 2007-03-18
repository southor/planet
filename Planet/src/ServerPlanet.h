#ifndef __ServerPlanet_h__
#define __ServerPlanet_h__

#include "Planet.h"
#include "ServerGlobalAccess.h"

namespace Planet
{
	class ServerPlanet : public Planet, public ServerGlobalAccess
	{
	public:
	};
};

#endif
