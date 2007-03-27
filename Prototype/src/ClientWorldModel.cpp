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

	void ClientWorldModel::addPlayerObj(PlayerId playerId, const Pos &playerPos, bool isMe, int tick)
	{		
		playerObjs.add(playerId, new PlayerObj(playerId, playerPos, (isMe ? CLIENT_PREDICTION_N_HISTORY_TICKS : CLIENT_INTERPOOLATION_N_HISTORY_TICKS), tick));
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
		//ForEach(getPlayerObjs().begin(), getPlayerObjs().end(), move);
	}

	void ClientWorldModel::handlePlayerShooting(PlayerId playerId)
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
			
			//Tickf shootTick = playerObj->getShotTick(currentTick, i);
			//GameObjId projectileId = playerShoot(playerId, weapon, shootTick, userCmd.objLag);
			//Projectile *projectile = (getProjectiles())[projectileId];
			GameObjId projectileId = getIdGenerator()->generateGameObjId();
			assert(projectileId == (userCmd.firstProjectileId + i));
			//if (
				playerTryShoot(playerId, currentTick, i, projectileId);
			//{		
			//	Projectile *projectile = (getProjectiles())[projectileId];
			//	std::cout << "server tick: " << getTimeHandler()->getTick() << "    projectile shot, objLag =  " << projectile->getObjLag() << std::endl;
			//	
			//	// send projectile to all clients
			//	AddProjectile addProjectile(projectileId, projectile->getType(), projectile->getPos(),
			//								projectile->getAngle().getFloat(), projectile->getShooterId(),
			//								projectile->getShootTick(), projectile->getObjLag());
			//	pushMessageToAll(players, addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//	//ServerPlayers::Iterator it = players.begin();
			//	//ServerPlayers::Iterator end = players.end();
			//	//for(; it != end; ++it)
			//	//{
			//	//	if (it->first != playerId) it->second->link.pushMessage(addProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//	//}
			//}
			//else
			//{
			//	RemoveProjectile removeProjectile(projectileId);
			//	players[playerId]->link.pushMessage(removeProjectile, getTimeHandler()->getTime(), getTimeHandler()->getTick());
			//}
		}		
	}

};
