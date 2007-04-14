#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"
#include "ServerGlobalAccess.h"

#include <vector>

namespace Prototype
{


	class WorldRenderer;

	class ServerWorldModel : public WorldModel, public ServerGlobalAccess
	{
	private:

		Obstacles obstacles;
		PlayerObjs playerObjs;
		Projectiles projectiles;

		typedef std::vector<Pos> RespawnPoss;
		RespawnPoss respawnPoss;

	public:

		ServerWorldModel(ServerGlobalObj *serverGlobalObj)
			: ServerGlobalAccess(serverGlobalObj)
		{}

		~ServerWorldModel();
		
		Obstacles& getObstacles()						{ return obstacles; }
		PlayerObjs& getPlayerObjs()						{ return playerObjs; }
		Projectiles& getProjectiles()					{ return projectiles; }

		const Obstacles& getObstacles() const			{ return obstacles; }
		const PlayerObjs& getPlayerObjs() const			{ return playerObjs; }
		const Projectiles& getProjectiles() const		{ return projectiles; }


		void addPlayerObj(PlayerId playerId, const Pos &playerPos, int fullHealth);
		GameObjId addObstacle(const Rectangle &obstacleArea);

		void addRespawnPos(Pos pos)						{ respawnPoss.push_back(pos); }
		
		void updatePlayerObjMovements();
		void updateProjectileMovements();
		
		void performProjectileHits(ServerPlayers &players, WorldRenderer *worldRenderer);

		void handlePlayerShooting(PlayerId playerId, ServerPlayers &players);
	};
};

#endif
