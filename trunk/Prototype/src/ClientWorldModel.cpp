#include "ClientWorldModel.h"
#include "ClientTimeHandler.h"

#include <algorithm>

namespace Prototype
{
	ClientWorldModel::~ClientWorldModel()			
	{
		deleteAllObjs();		
	}

	void ClientWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos, bool isMe, int tick, int fullHealth)
	{		
		playerObjs.add(playerId, new PlayerObj(playerId, playerPos, (isMe ? CLIENT_PREDICTION_N_HISTORY_TICKS : CLIENT_INTERPOOLATION_N_HISTORY_TICKS), tick, fullHealth));
	}

	void ClientWorldModel::addObstacle(GameObjId obstacleId, const Rectangle &obstacleArea)
	{
		obstacles.add(obstacleId, new Obstacle(obstacleArea));
	}

	void ClientWorldModel::addProjectile(GameObjId projectileId, Projectile::Type type, const Pos &pos, float angle, PlayerId shooterId, Tickf shootTick, int objLag)
	{
		Projectile *projectile = new Projectile(type, pos, angle, shooterId, CLIENT_INTERPOOLATION_N_HISTORY_TICKS, shootTick, objLag);
		assert(projectile);
		if (projectile) projectiles.add(projectileId, projectile);
	}

	void ClientWorldModel::updatePlayerObjMovement(PlayerId playerId)
	{
		MovePlayerObj move(&getObstacles(), moveAlignedToAngle);
		PlayerObjs::Pair playerObjPair(playerId, (getPlayerObjs())[playerId]);
		move(playerObjPair);
	}

	void ClientWorldModel::handlePlayerShooting(PlayerId playerId, bool createProjectile)
	{
		int currentTick = static_cast<int>(getTimeHandler()->getStepTick());
		Tickf currentTickf = static_cast<Tickf>(currentTick);

		PlayerObj *playerObj = getPlayerObjs()[playerId];
		const UserCmd &userCmd(playerObj->getUserCmd());
		
		assert((userCmd.firstShotTick >= playerObj->getNextShootTick()) &&
				(playerObj->getNextShootTick() >= currentTickf));
		
		// Do any shooting
		int nShots = userCmd.nShots;
		Projectile::Type weapon = userCmd.weapon;
		for(int i=0; i<nShots; ++i)
		{
			GameObjId projectileId = getIdGenerator()->generateGameObjId();
			assert(projectileId == (userCmd.firstProjectileId + i));

			if (createProjectile) playerTryShoot(playerId, currentTick, i, projectileId);
		}		
	}

};
