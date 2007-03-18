#include "ClientPlanet.h"

namespace Planet
{
	void ClientPlanet::addProjectile(GameObjId projectileId, AddProjectile *addProjectile)
	{
		projectiles.add(projectileId, new Projectile(addProjectile->type, addProjectile->pos, addProjectile->direction, addProjectile->shooterId, CLIENT_INTERPOOLATION_N_HISTORY_TICKS, addProjectile->shootTick, addProjectile->objLag));
	}
};