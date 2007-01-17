#include "clientworldmodel.h"
#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		deleteAllObjs();
	}

	//void ClientWorldModel::addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos)
	//{
	//	playerObjs.add(playerObjId, new PlayerObj(playerId, playerPos));
	//}

	void ClientWorldModel::addPlayerObj(size_t playerId, const Pos &playerPos)
	{
		playerObjs.add(playerId, new PlayerObj(playerPos));
	}

	void ClientWorldModel::addObstacle(size_t obstacleId, const Rectangle &obstacleArea)
	{
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
	}

	void ClientWorldModel::addProjectile(size_t projectileId, Projectile::Type type, const Pos &pos, float angle)
	{
		projectiles.add(projectileId, new Projectile(type, pos, angle));
	}

};