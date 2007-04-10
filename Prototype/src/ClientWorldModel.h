#ifndef __clientworldmodel_h__
#define __clientworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"
#include "ClientGlobalAccess.h"

namespace Prototype
{
	class ClientWorldModel : public WorldModel, public ClientGlobalAccess
	{
	private:
		

		//typedef IdMap<GameObjId, Obstacle*> ClientObstacles;
		//typedef IdMap<GameObjId, PlayerObj*> ClientPlayerObjs;
		//typedef IdMap<GameObjId, Projectile*> ClientProjectiles;

		Obstacles obstacles;
		PlayerObjs playerObjs;
		Projectiles projectiles;

	public:

		ClientWorldModel(ClientGlobalObj *clientGlobalObj) : ClientGlobalAccess(clientGlobalObj)
		{}

		~ClientWorldModel();
				
		Obstacles& getObstacles()						{ return obstacles; }
		PlayerObjs& getPlayerObjs()						{ return playerObjs; }
		Projectiles& getProjectiles()					{ return projectiles; }

		const Obstacles& getObstacles() const			{ return obstacles; }
		const PlayerObjs& getPlayerObjs() const			{ return playerObjs; }
		const Projectiles& getProjectiles() const		{ return projectiles; }


		//void addPlayerObj(size_t playerId, size_t playerObjId, const Pos &playerPos);
		void addPlayerObj(PlayerId playerId, const Pos &playerPos, bool isMe, int tick, int fullHealth);
		void addObstacle(GameObjId obstacleId, const Rectangle &obstacleArea);
		void addProjectile(GameObjId projectileId, Projectile::Type type, const Pos &pos, float angle, PlayerId shooterId, Tickf shootTick, int objLag);

		void updatePlayerObjMovement(PlayerId playerId);

		void handlePlayerShooting(PlayerId playerId, bool createProjectile);

	};
};

#endif
