#include "ClientWorldModel.h"
#include "ClientTimeHandler.h"

#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		deleteAllObjs();
	}

	//void ClientWorldModel::addPlayerObj(PlayerId playerId, size_t playerObjId, const Pos &playerPos)
	//{
	//	playerObjs.add(playerObjId, new PlayerObj(playerId, playerPos));
	//}

	void ClientWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos, bool isMe)
	{		
		playerObjs.add(playerId, new PlayerObj(playerPos, (isMe ? CLIENT_PREDICTION_N_HISTORY_TICKS : CLIENT_INTERPOOLATION_N_HISTORY_TICKS)));
	}

	void ClientWorldModel::addObstacle(GameObjId obstacleId, const Rectangle &obstacleArea)
	{
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
	}

	void ClientWorldModel::addProjectile(GameObjId projectileId, Projectile::Type type, const Pos &pos, float angle, PlayerId shooterId)
	{
		projectiles.add(projectileId, new Projectile(type, pos, angle, shooterId, CLIENT_INTERPOOLATION_N_HISTORY_TICKS));
	}

};
