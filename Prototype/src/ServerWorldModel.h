#ifndef __serverworldmodel_h__
#define __serverworldmodel_h__

#include "WorldModel.h"
#include "IdMap.h"
//#include "ServerPlayers.h"
#include "ServerGlobalAccess.h"

#include <vector>

namespace Prototype
{


	class WorldRenderer;

	class ServerWorldModel : public WorldModel, public ServerGlobalAccess
	{
	private:


/*		typedef IdMap<GameObjId, Obstacle*> ServerObstacles;
		typedef IdMap<GameObjId, PlayerObj*> ServerPlayerObjs;
		typedef IdMap<GameObjId, Projectile*> ServerProjectiles;	*/	

		Obstacles obstacles;
		PlayerObjs playerObjs;
		Projectiles projectiles;

		typedef std::vector<Pos> RespawnPoss;
		RespawnPoss respawnPoss;



		//class MoveProjectile : public Move, public ServerGlobalAccess
		//{
		//private:

		//	RespawnPoss *respawnPoss;
		//	ServerPlayers *players;
		//	PlayerObjs *playerObjs;

		//	std::vector<RemoveProjectile> projectilesHit;

		//public:
		//	// @param deltaTime Time in milliseconds since last move.
		//	MoveProjectile(Obstacles *obstacles, ServerPlayers *players, const ServerGlobalAccess &serverGlobalAccess, PlayerObjs *playerObjs, RespawnPoss *respawnPoss)
		//		: Move(obstacles), ServerGlobalAccess(serverGlobalAccess), respawnPoss(respawnPoss), players(players), playerObjs(playerObjs)
		//	{}

		//	void operator ()(const Projectiles::Pair &projectilePair);

		//	inline std::vector<RemoveProjectile>& getProjectilesHit()	
		//	{
		//		return projectilesHit;
		//	}
		//};

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
		
		//bool performProjectileHit(GameObjId projectileId, ServerPlayers &players, ProjectileHit &projectileHit);		
		void performProjectileHits(ServerPlayers &players, WorldRenderer *worldRenderer);



		void handlePlayerShooting(PlayerId playerId, ServerPlayers &players);
	};
};

#endif
